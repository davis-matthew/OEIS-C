/*
 * A089306  Smallest prime of the form n + (n+1)+ (n+2)+...+(n+k), or 0 if no
 *          such prime exists.
 * https://oeis.org/A089306
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n if n is prime, else a(n) = 2*n+1 if that is prime, else
 *            0
 * Range:     u8 n <= 127, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* 64‑bit modular multiplication (no overflow) */
static inline uint64_t mul_mod64(uint64_t a, uint64_t b, uint64_t mod)
{
    return (uint64_t)((__int128)a * b % mod);
}

/* 64‑bit modular exponentiation */
static uint64_t pow_mod64(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t result = 1;
    while (exp != 0) {
        if (exp & 1)
            result = mul_mod64(result, base, mod);
        base = mul_mod64(base, base, mod);
        exp >>= 1;
    }
    return result;
}

/* Deterministic Miller–Rabin test for 64‑bit integers */
static bool is_prime_u64(uint64_t n)
{
    if (n < 2)
        return false;
    static const uint64_t small_primes[] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37};
    for (size_t i = 0; i < sizeof(small_primes) / sizeof(small_primes[0]); ++i) {
        if (n == small_primes[i])
            return true;
        if (n % small_primes[i] == 0)
            return false;
    }

    /* write n‑1 as d·2^s with d odd */
    uint64_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        ++s;
    }

    /* deterministic bases for 64‑bit numbers */
    const uint64_t bases[] = {
        2ULL, 325ULL, 9375ULL, 28178ULL,
        450775ULL, 9780504ULL, 1795265022ULL
    };

    for (size_t i = 0; i < sizeof(bases) / sizeof(bases[0]); ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0)
            continue;
        uint64_t x = pow_mod64(a, d, n);
        if (x == 1 || x == n - 1)
            continue;
        bool composite = true;
        for (unsigned r = 1; r < s; ++r) {
            x = mul_mod64(x, x, n);
            if (x == n - 1) {
                composite = false;
                break;
            }
        }
        if (composite)
            return false;
    }
    return true;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;          /* invalid index → 0 */

    seq_t nseq = (seq_t)n;

    /* 1. If n is prime, return n */
    if (nseq <= (seq_t)UINT64_MAX && is_prime_u64((uint64_t)nseq))
        return nseq;

    /* 2. If 2*n+1 is prime, return 2*n+1 */
    seq_t two_n_plus_one = (seq_t)2 * nseq + (seq_t)1;
    if (two_n_plus_one <= (seq_t)UINT64_MAX &&
        is_prime_u64((uint64_t)two_n_plus_one))
        return two_n_plus_one;

    /* 3. Otherwise return 0 */
    return (seq_t)0;
}
