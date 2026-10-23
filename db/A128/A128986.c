/*
 * A128986  a(n) = (n^3 - n^2)*3^n.
 * https://oeis.org/A128986
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^3 - n^2)*3^n = n^2*(n-1)*3^n
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 13, u64 n <= 31, u128 n <= 69,
 *            big any n
 * Helpers:   pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow3.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    seq_t p = pow3(n);
    seq_t n_sq = (seq_t)n * (seq_t)n;
    seq_t term = n_sq * (seq_t)(n - 1);
    return term * p;
}
