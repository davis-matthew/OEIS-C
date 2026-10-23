/*
 * A101084  Numbers k such that 97*k + 101 is a prime.
 * https://oeis.org/A101084
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth integer k such that 97*k + 101 is prime
 * Range:     u8 n <= 28, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Deterministic Miller–Rabin test for 64‑bit integers is used. */

#include <stdint.h>
#include <stdbool.h>

/* 64‑bit modular multiplication using 128‑bit intermediate */
static uint64_t mod_mul_uint64(uint64_t a, uint64_t b, uint64_t m)
{
    return (unsigned __int128)a * b % m;
}

/* 64‑bit modular exponentiation */
static uint64_t mod_pow_uint64(uint64_t a, uint64_t e, uint64_t m)
{
    uint64_t r = 1;
    while (e) {
        if (e & 1ULL) r = mod_mul_uint64(r, a, m);
        a = mod_mul_uint64(a, a, m);
        e >>= 1;
    }
    return r;
}

/* Deterministic Miller–Rabin for all 64‑bit numbers */
static bool is_prime_u64(uint64_t n)
{
    if (n < 2) return false;
    static const uint64_t small_primes[3] = {2ULL, 3ULL, 5ULL};
    unsigned i;
    for (i = 0; i < sizeof(small_primes)/sizeof(small_primes[0]); ++i) {
        uint64_t p = small_primes[i];
        if (n == p) return true;
        if (n % p == 0ULL) return false;
    }

    uint64_t d = n - 1;
    unsigned s = 0;
    while ((d & 1ULL) == 0ULL) {
        d >>= 1;
        ++s;
    }

    static const uint64_t bases[7] = {
        2ULL, 325ULL, 9375ULL, 28178ULL,
        450775ULL, 9780504ULL, 1795265022ULL
    };
    for (i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0) continue;
        uint64_t x = mod_pow_uint64(a, d, n);
        if (x == 1ULL || x == n - 1) continue;
        bool composite = true;
        unsigned r;
        for (r = 1; r < s; ++r) {
            x = mod_mul_uint64(x, x, n);
            if (x == n - 1) { composite = false; break; }
        }
        if (composite) return false;
    }
    return true;
}

/* Return the nth term of A101084 (1‑based indexing). */
seq_t a(idx_t n)
{
    if (n == 0) return 0; /* out of bounds, but harmless */
    seq_t k = 0;
    idx_t found = 0;
    for (;;) {
        uint64_t val = 97ULL * (uint64_t)k + 101ULL;
        if (is_prime_u64(val)) {
            ++found;
            if ((idx_t)found == n) return (seq_t)k;
        }
        ++k;
    }
}
