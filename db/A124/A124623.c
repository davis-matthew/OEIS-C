/*
 * A124623  Number of unit squares having center within inscribed circle of an
 *          n X n integer square.
 * https://oeis.org/A124623
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 288, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: count unit squares whose centers lie within
 * the inscribed circle of an n×n unit square grid. */
#include <stdint.h>

static seq_t isqrt_u64(idx_t n)
{
    idx_t lo = 0, hi = n;
    while (lo < hi) {
        idx_t mid = (lo + hi + 1) >> 1;
        unsigned __int128 prod = (unsigned __int128)mid * mid;
        if (prod <= n)
            lo = mid;
        else
            hi = mid - 1;
    }
    return (seq_t)lo;
}

seq_t a(idx_t n)
{
    seq_t result = 0;
    if (n == 0) return result; /* not used but safe */

    /* pre‑compute n² once */
    unsigned __int128 nn128 = (unsigned __int128)n * n;
    idx_t nn = (idx_t)nn128;

    for (idx_t i = 0; i < n; ++i) {
        idx_t i2 = 2 * i + 1;           /* scaled x‑coordinate (odd) */
        idx_t dxi = (i2 >= n) ? (i2 - n) : (n - i2); /* |i2 – n| */
        unsigned __int128 dxi2 = (unsigned __int128)dxi * dxi;
        if (dxi2 > nn128) continue;     /* no points in this column */

        unsigned __int128 rhs128 = nn128 - dxi2;
        idx_t m = (idx_t) isqrt_u64((idx_t)rhs128); /* floor sqrt(RHS) */

        idx_t L = (n > m) ? (n - m) : 0;
        idx_t R = n + m;
        if (L < 1) L = 1;
        if (R > 2 * n - 1) R = 2 * n - 1;

        /* count odd numbers in [L, R] */
        idx_t count = ((R + 1) / 2) - (L / 2);
        result += (seq_t)count;
    }
    return result;
}
