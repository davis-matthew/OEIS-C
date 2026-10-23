/*
 * A013944  Sum of terms in period of continued fraction for square root of
 *          the n-th nonsquare.
 * https://oeis.org/A013944
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 56, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) = sum of the terms of the periodic part of the simple continued fraction of √(nth nonsquare integer). */
#include "../utils/floor_sqrt.h"

static seq_t nth_nonsquare(idx_t n) {
    idx_t count = 0;
    for (idx_t x = 1; ; ++x) {
        seq_t a0 = floor_sqrt((seq_t)x);
        if (a0 * a0 != (seq_t)x) {          /* x is not a perfect square */
            ++count;
            if (count == n)
                return (seq_t)x;
        }
    }
}

static seq_t period_sum(seq_t x) {
    seq_t a0 = floor_sqrt(x);
    seq_t sum = 0;
    seq_t m = 0;
    seq_t d = 1;
    seq_t a = a0;
    while (1) {
        m = d * a - m;
        d = (x - m * m) / d;
        a = (a0 + m) / d;
        sum += a;
        if (a == 2 * a0)
            break;
    }
    return sum;
}

seq_t a(idx_t n) {
    seq_t nonsquare = nth_nonsquare(n);
    return period_sum(nonsquare);
}
