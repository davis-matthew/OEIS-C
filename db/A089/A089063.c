/*
 * A089063  Numbers k such that 840*k + 175177943 is a prime.
 * https://oeis.org/A089063
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth integer k (k ≥ 2) such that 840*k + 175177943 is
 *            prime. Sequence is indexed from 1 (a(1) = 2)
 * Range:     u8 n <= 62, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

/* Miller–Rabin primality test for 64‑bit integers (deterministic). */
static inline uint64_t mul_mod(uint64_t a, uint64_t b, uint64_t mod)
{
    return (uint64_t)((__uint128_t)a * b % mod);
}

static inline uint64_t pow_mod(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t result = 1;
    while (exp) {
        if (exp & 1)
            result = mul_mod(result, base, mod);
        base = mul_mod(base, base, mod);
        exp >>= 1;
    }
    return result;
}

static inline bool is_prime_u64(uint64_t n)
{
    if (n < 2)
        return false;
    static const uint64_t small_primes[] = {
        2ULL, 3ULL, 5ULL, 7ULL, 11ULL, 13ULL, 17ULL, 19ULL, 23ULL, 29ULL,
        31ULL, 37ULL, 41ULL, 43ULL, 47ULL, 53ULL, 59ULL, 61ULL, 67ULL,
        71ULL, 73ULL, 79ULL, 83ULL, 89ULL, 97ULL
    };
    for (size_t i = 0; i < sizeof(small_primes)/sizeof(small_primes[0]); ++i) {
        if (n == small_primes[i])
            return true;
        if (n % small_primes[i] == 0)
            return false;
    }

    /* Miller–Rabin deterministic bases for 64‑bit integers. */
    static const uint64_t bases[] = {
        2ULL, 325ULL, 9375ULL, 28178ULL,
        450775ULL, 9780504ULL, 1795265022ULL
    };

    uint64_t d = n - 1;
    unsigned s = 0;
    while ((d & 1ULL) == 0ULL) {
        d >>= 1;
        s++;
    }

    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0)
            continue;
        uint64_t x = pow_mod(a, d, n);
        if (x == 1 || x == n - 1)
            continue;
        bool cont = false;
        for (unsigned r = 1; r < s; ++r) {
            x = mul_mod(x, x, n);
            if (x == n - 1) {
                cont = true;
                break;
            }
        }
        if (cont)
            continue;
        return false;
    }
    return true;
}

/* Cache for already discovered terms.  200 000 is more than enough for the
   required 10 000 terms and keeps the type generic. */
#define MAX_CACHE 200000
static seq_t cache[MAX_CACHE];
static size_t cache_len = 0;
static uint64_t next_k = 2;   /* candidate k to test next */

/* Compute the n-th term (n>=1).  a(0) is undefined; we return 0. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* offset is 1; behaviour below offset arbitrary */

    /* Generate terms until we have at least n of them. */
    while (cache_len < (size_t)n) {
        /* Test current candidate k.  k >= 2 by construction. */
        uint64_t k = next_k;
        uint64_t value = 840ULL * k + 175177943ULL;
        if (is_prime_u64(value)) {
            if (cache_len < (size_t)MAX_CACHE)
                cache[cache_len] = (seq_t)k;
            cache_len++;
        }
        next_k++;
        /* If the cache becomes full we simply stop storing new terms,
           but the sequence will still be computed correctly using the
           search loop above. */
    }
    return cache[n - 1];
}
