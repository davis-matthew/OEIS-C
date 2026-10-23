/*
 * A106574  Indices n of perfect squares n^2 which are neither the sum nor the
 *          difference of two primes.
 * https://oeis.org/A106574
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = the n-th odd integer m such that m^2-2 and m^2+2 are
 *            composite. Equivalently, a(n) = sqrt( A106573(n) )
 * Range:     u8 n <= 56, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* 64‑bit modular multiplication using 128‑bit intermediate */
static inline uint64_t modmul64(uint64_t a, uint64_t b, uint64_t mod)
{
    return (unsigned __int128)a * b % mod;
}

/* 64‑bit modular exponentiation */
static inline uint64_t modpow64(uint64_t a, uint64_t e, uint64_t mod)
{
    uint64_t r = 1;
    while (e) {
        if (e & 1ULL) r = modmul64(r, a, mod);
        a = modmul64(a, a, mod);
        e >>= 1;
    }
    return r;
}

/* 64‑bit deterministic Miller–Rabin primality test.
   Valid for all 64‑bit unsigned integers. */
static inline bool is_prime64(uint64_t n)
{
    if (n < 2) return false;
    /* small prime trial division */
    static const uint64_t small[] = {
        2ULL, 3ULL, 5ULL, 7ULL, 11ULL, 13ULL, 17ULL, 19ULL, 23ULL, 29ULL,
        31ULL, 37ULL, 41ULL, 43ULL, 47ULL, 53ULL, 59ULL, 61ULL, 67ULL,
        71ULL, 73ULL, 79ULL, 83ULL, 89ULL, 97ULL
    };
    for (size_t i = 0; i < sizeof(small)/sizeof(*small); ++i) {
        if (n == small[i]) return true;
        if (n % small[i] == 0) return false;
    }

    /* write n-1 = d * 2^s */
    uint64_t d = n - 1;
    unsigned s = 0;
    while ((d & 1ULL) == 0ULL) {
        d >>= 1;
        ++s;
    }

    /* bases that are deterministic for 64‑bit */
    static const uint64_t bases[] = {2ULL, 325ULL, 9375ULL, 28178ULL,
                                     450775ULL, 9780504ULL, 1795265022ULL};
    for (size_t i = 0; i < sizeof(bases)/sizeof(*bases); ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0) continue;
        uint64_t x = modpow64(a, d, n);
        if (x == 1 || x == n - 1) continue;
        bool composite = true;
        for (unsigned r = 1; r < s; ++r) {
            x = modmul64(x, x, n);
            if (x == n - 1) {
                composite = false;
                break;
            }
        }
        if (composite) return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    idx_t count = 0;
    idx_t m = 3;        /* start with the first odd integer >=3 */
    while (1) {
        if ((m & 1ULL) == 0) {           /* skip even m (not needed but keeps loop simple) */
            ++m;
            continue;
        }
        uint64_t sq = (uint64_t)m * m;   /* m^2, < 2^64 for the required n */
        /* prime tests for m^2-2 and m^2+2 */
        bool left_prime  = is_prime64(sq - 2);
        bool right_prime = is_prime64(sq + 2);

        if (!left_prime && !right_prime) {
            ++count;
            if (count == n)
                return (seq_t)m;
        }
        ++m;
    }
}
