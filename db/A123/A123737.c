/*
 * A123737  Partial sums of (-1)^floor(n*sqrt(2)).
 * https://oeis.org/A123737
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = sum_{k=1}^n (-1)^{floor(k*sqrt(2))}
 * Range:     i8 n <= 5, i16 n <= 90, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* We compute floor(k*sqrt(2)) as floor(sqrt(2*k*k)) using an exact integer sqrt. */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t i = 1; i <= n; ++i) {
        seq_t k = (seq_t)i;
        seq_t m = k * k * 2;          /* 2 * k^2, exact arithmetic in seq_t */
        seq_t s = floor_sqrt(m);      /* floor(k*sqrt(2)) */
        seq_t parity = s & 1;         /* 0 if even, 1 if odd */
        seq_t term = 1 - (parity << 1);/* 1 when s even, -1 when s odd */
        sum += term;
    }
    return sum;
}
