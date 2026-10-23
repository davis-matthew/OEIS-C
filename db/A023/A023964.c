/*
 * A023964  First digit after decimal point of 5th root of n.
 * https://oeis.org/A023964
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = floor(10*n^(1/5)) mod 10
 * Range:     u8 none, u16 none, u32 n <= 1000, u64 n <= 1000, u128 n <= 1000,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/seq_pow.h"

static seq_t floor_fifth_root(seq_t x)
{
    seq_t lo = 0;
    seq_t hi = 1;

    /* Find an upper bound for the 5th root */
    while (true) {
        seq_t p = seq_pow(hi, (idx_t)5);
        if (p > x)
            break;
        lo = hi;
        hi = hi << 1;      /* hi *= 2 */
    }

    /* Binary search between lo (≤ root) and hi (> root) */
    while (lo + 1 < hi) {
        seq_t mid = (lo + hi) >> 1;   /* mid = (lo + hi) / 2 */
        seq_t p   = seq_pow(mid, (idx_t)5);
        if (p <= x)
            lo = mid;
        else
            hi = mid;
    }
    return lo;
}

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;

    /* 10^5 as seq_t */
    const seq_t POW10_5 = seq_pow((seq_t)10, (idx_t)5);

    /* floor(10 * n^(1/5)) = floor((10^5 * n)^(1/5)) */
    seq_t t = floor_fifth_root(POW10_5 * N);

    /* floor(n^(1/5)) */
    seq_t r = floor_fifth_root(N);

    /* First decimal digit: t - 10*r */
    seq_t d = t - ((seq_t)10 * r);
    return d;  /* d lies in [0,9] */
}
