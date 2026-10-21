/*
 * A233196  Number of n X 2 0..7 arrays with no element x(i,j) adjacent to
 *          itself or value 7-x(i,j) horizontally, antidiagonally or
 *          vertically, top left element zero, and 1 appearing before 2 3 4 5
 *          and 6, 2 appearing before 3 4 and 5, and 3 appearing before 4 in
 *          row major order (unlabeled 8-colorings with no...
 * https://oeis.org/A233196
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^(2*n-7) * (4^n + 8) for n > 1, with a(1) = 1
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 17, u128 n <= 33,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements the closed‑form from the OEIS conjecture.            */

#include <stdlib.h>
#include "../utils/pow2.h"   /* pow2(idx_t exp) -> seq_t */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;

    /* 4^n = 2^(2*n) */
    seq_t val = pow2(2 * n) + (seq_t)8;

    /* diff = 7 - 2*n; may be negative.   */
    long long diff = 7LL - (long long)(2 * n);

    if (diff > 0) {
        /* divide by 2^diff */
        val /= pow2((idx_t)diff);
    } else if (diff < 0) {
        /* multiply by 2^(-diff) */
        val *= pow2((idx_t)(-diff));
    }
    return val;
}
