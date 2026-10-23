/*
 * A049611  a(n) = T(n,2), array T as in A049600.
 * https://oeis.org/A049611
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(n-4)*(n^2+5*n+2) (n ≥ 1), a(0) = 0 Equivalent to a(n)
 *            = (n*(n+5)+2) / 2^(4-n) for n < 4
 * Range:     u8 n <= 5, u16 n <= 12, u32 n <= 26, u64 n <= 56, u128 n <= 118,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* numerator = n*(n+5)+2, computed in seq_t to avoid overflow */
    seq_t num = (seq_t)n * ((seq_t)n + 5) + 2;

    if (n >= 4)
        return pow2(n - 4) * num;          /* left shift equivalent */
    else
        return num / pow2(4 - n);          /* right shift equivalent */
}
