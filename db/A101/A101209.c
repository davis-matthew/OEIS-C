/*
 * A101209  Smallest odd prime p such that n=(p-1)/ord_p(3).
 * https://oeis.org/A101209
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest odd prime p such that n = (p-1)/ord_p(3)
 * Range:     u8 n <= 6, u16 n <= 30, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   is_prime, pow_mod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation iterates over possible orders m, computes candidate p = n*m+1,
   checks that p is prime, and verifies that   3^m ≡ 1 (mod p)  and that the
   multiplicative order of 3 modulo p equals m. */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/pow_mod.h"

static uint64_t order_of_3(uint64_t p, uint64_t m)
{
    /* 3 must be coprime to p (p > 3) so we can test its order. */
    if (pow_mod(3ULL, m, p) != 1ULL)
        return 0;                /* m cannot be the order */

    uint64_t ord = m;
    uint64_t tmp = m;

    /* Factor m and reduce ord by its prime factors where possible. */
    for (uint64_t f = 2; f * f <= tmp; ++f) {
        if (tmp % f != 0)
            continue;
        while (tmp % f == 0)
            tmp /= f;
        while (ord % f == 0 &&
               pow_mod(3ULL, ord / f, p) == 1ULL)
            ord /= f;
    }
    if (tmp > 1) {                 /* remaining prime factor */
        uint64_t f = tmp;
        while (ord % f == 0 &&
               pow_mod(3ULL, ord / f, p) == 1ULL)
            ord /= f;
    }
    return ord;
}

seq_t a(idx_t n)
{
    uint64_t nn = (uint64_t)n;

    for (uint64_t m = 1;; ++m) {
        __uint128_t prod = (__uint128_t)nn * m + 1ULL;
        if (prod > UINT64_MAX)
            break;                /* safety – should not be reached for tested range */
        uint64_t p = (uint64_t)prod;

        if (p <= 3 || (p & 1ULL) == 0)
            continue;
        if (!is_prime(p))
            continue;

        if (order_of_3(p, m) == m)
            return (seq_t)p;
    }

    /* Should never happen for valid n. */
    return (seq_t)0;
}
