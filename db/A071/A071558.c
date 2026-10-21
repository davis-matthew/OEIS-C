/*
 * A071558  Smallest k such that n*k + 1 and n*k - 1 are twin primes.
 * https://oeis.org/A071558
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = smallest k >= 1 such that n*k-1 and n*k+1 are both prime
 *            (twin primes)
 * Range:     u8 n <= 60, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   mod_mul64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>   /* for size_t */
#include "../utils/mod_mul64.h"

/*----- 64‑bit deterministic Miller–Rabin primality test -----*/

static inline uint64_t mod_pow64(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t res = 1;
    while (exp) {
        if (exp & 1) res = mod_mul64(res, base, mod);
        base = mod_mul64(base, base, mod);
        exp >>= 1;
    }
    return res;
}

static inline bool is_prime_uint64(uint64_t n)
{
    if (n < 2) return false;
    /* Trial division by small primes */
    static const uint8_t small_primes[] = {2, 3, 5, 7, 11, 13, 17, 19,
                                           23, 29, 31, 37};
    for (size_t i = 0; i < sizeof(small_primes)/sizeof(small_primes[0]); ++i) {
        uint64_t p = small_primes[i];
        if (n % p == 0) return n == p;
    }

    /* Decompose n-1 as d*2^s */
    uint64_t d = n - 1;
    int s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        ++s;
    }

    /* Deterministic witnesses for all 64-bit integers */
    static const uint64_t witnesses[] = {
        2ULL, 325ULL, 9375ULL, 28178ULL,
        450775ULL, 9780504ULL, 1795265022ULL
    };
    for (size_t i = 0; i < sizeof(witnesses)/sizeof(witnesses[0]); ++i) {
        uint64_t a = witnesses[i] % n;
        if (a == 0) continue;
        uint64_t x = mod_pow64(a, d, n);
        if (x == 1 || x == n - 1) continue;
        bool composite = true;
        for (int r = 1; r < s; ++r) {
            x = mod_mul64(x, x, n);
            if (x == n - 1) {
                composite = false;
                break;
            }
        }
        if (composite) return false;
    }
    return true;
}

/*----- Main sequence function -----*/
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* offset is 1, but guard for completeness */
    for (idx_t k = 1; ; ++k) {
        /* Compute n*k safely; for n, k up to 10^4 the product fits in 64 bits. */
        uint64_t prod = (uint64_t)n * (uint64_t)k;
        uint64_t m1 = prod > 0 ? prod - 1 : 0;   /* avoid underflow when prod==0 */
        uint64_t m2 = prod + 1;
        if (is_prime_uint64(m1) && is_prime_uint64(m2))
            return (seq_t)k;
    }
}
