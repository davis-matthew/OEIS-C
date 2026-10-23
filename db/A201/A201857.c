/*
 * A201857  Primes of the form 8n^2 - 5.
 * https://oeis.org/A201857
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 14, u32 n <= 1561, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   mod_mul_uint64, mod_pow_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: a(n) is the n-th prime of the form 8k²-5 */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/mod_mul_uint64.h"
#include "../utils/mod_pow_uint64.h"

/* small primes for quick rejection */
static const uint64_t small_primes[] =
    {2ULL,3ULL,5ULL,7ULL,11ULL,13ULL,17ULL,19ULL,23ULL,29ULL,31ULL,37ULL};

/* deterministic Miller–Rabin primality test for 64‑bit integers */
static bool is_prime_uint64(uint64_t n)
{
    if (n < 2) return false;
    for (size_t i = 0; i < sizeof(small_primes)/sizeof(small_primes[0]); ++i) {
        if (n % small_primes[i] == 0) return n == small_primes[i];
    }

    /* write n-1 as d*2^s */
    uint64_t d = n - 1;
    unsigned int s = 0;
    while ((d & 1U) == 0U) {
        d >>= 1U;
        ++s;
    }

    /* bases sufficient for 64‑bit */
    const uint64_t bases[] = {2ULL,325ULL,9375ULL,28178ULL,450775ULL,9780504ULL,1795265022ULL};
    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0ULL) return true;
        uint64_t x = mod_pow_uint64(a, d, n);
        if (x == 1ULL || x == n - 1) continue;
        bool composite = true;
        for (unsigned r = 1U; r < s; ++r) {
            x = mod_mul_uint64(x, x, n);
            if (x == n - 1) { composite = false; break; }
        }
        if (composite) return false;
    }
    return true;
}

/* compute the n-th term of the sequence */
seq_t a(idx_t n)
{
    idx_t found = 0;
    for (idx_t k = 1; ; ++k) {
        /* compute 8*k^2 - 5 as a seq_t value */
        seq_t val = (seq_t)8 * k * k - (seq_t)5;

        /* test primality of the 64‑bit integer representation */
        uint64_t v64 = (uint64_t)val;
        if (is_prime_uint64(v64)) {
            ++found;
            if (found == n) return val;
        }
    }
}
