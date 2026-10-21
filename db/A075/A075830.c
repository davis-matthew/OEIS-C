/*
 * A075830  Let u(1) = x and u(n+1) = (n^2/u(n)) + 1 for n >= 1; then a(n) is
 *          such that u(n) = (b(n)*x + c(n))/(a(n)*x + d(n)) (in lowest terms)
 *          and a(n), b(n), c(n), d(n) are positive integers.
 * https://oeis.org/A075830
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the denominator (coefficient of x) in the continued
 *            fraction representation of u(1) = x, u(n+1) = n^2 / u(n) + 1. It
 *            satisfies the recurrence: starting from a = 0, b = 1, c = 0, d =
 *            1 for k = 1,2,…,n-1 n_sq = k^2 na = b nb = n_sq * a + b nc =
 *            n_sq * d + c nd = b g = gcd(na, nb, nc, nd) a...
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 25, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#if defined(__SIZEOF_INT128__)
typedef unsigned __int128 uint128_t;
#endif

#include "../utils/gcd.h"

#if defined(__SIZEOF_INT128__)
static inline uint128_t gcd128_uint128(uint128_t a, uint128_t b)
{
    while (b) {
        uint128_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}
#endif

seq_t a(idx_t n)
{
    /* a(1) is 0 */
    if (n == (idx_t)1) return (seq_t)0;

#if defined(__SIZEOF_INT128__)
    /* 128‑bit path for integral seq_t (uint8_t,…,uint64_t) */
    uint128_t a_val = 0, b_val = 1, c_val = 0, d_val = 1;
    for (idx_t k = 1; k < n; ++k) {
        uint128_t n_sq = (uint128_t)k * (uint128_t)k;
        uint128_t na = b_val;
        uint128_t nb = n_sq * a_val + b_val;
        uint128_t nc = n_sq * d_val + c_val;
        uint128_t nd = b_val;
        /* compute gcd of the four numbers */
        uint128_t g = gcd128_uint128(na, nb);
        g = gcd128_uint128(g, nc);
        g = gcd128_uint128(g, nd);
        /* reduce */
        a_val = na / g;
        b_val = nb / g;
        c_val = nc / g;
        d_val = nd / g;
    }
    return (seq_t)a_val;
#else
    /* Arbitrary‑precision path (seq_t may be a class) */
    seq_t a_val = 0, b_val = 1, c_val = 0, d_val = 1;
    for (idx_t k = 1; k < n; ++k) {
        seq_t n_sq = (seq_t)k * (seq_t)k;
        seq_t na = b_val;
        seq_t nb = n_sq * a_val + b_val;
        seq_t nc = n_sq * d_val + c_val;
        seq_t nd = b_val;
        seq_t g = gcd(na, nb);
        g = gcd(g, nc);
        g = gcd(g, nd);
        a_val = na / g;
        b_val = nb / g;
        c_val = nc / g;
        d_val = nd / g;
    }
    return a_val;
#endif
}
