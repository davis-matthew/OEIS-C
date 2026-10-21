/*
 * A233164  Number of n X 4 0..7 arrays with no element x(i,j) adjacent to
 *          itself or value 7-x(i,j) horizontally, diagonally, antidiagonally
 *          or vertically, top left element zero, and 1 appearing before 2 3 4
 *          5 and 6, 2 appearing before 3 4 and 5, and 3 appearing before 4 in
 *          row major order (unlabelled 8-colo...
 * https://oeis.org/A233164
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4^(2*n-3)*(2^n+12) for n>1; a(1)=11
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 26,
 *            big any n
 * Helpers:   pow2, pow4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/pow4.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* should never be called with n<1 */
    if (n == 1) return (seq_t)11;  /* base case */

    /* exponent for 4^(2*n-3) */
    idx_t exp4 = (n << 1) - 3;     /* 2*n - 3 */

    seq_t pow4_val = pow4(exp4);   /* 4^(2*n-3) */
    seq_t pow2_val = pow2(n);      /* 2^n */
    seq_t sum = pow2_val + (seq_t)12; /* 2^n + 12 */

    return pow4_val * sum;         /* 4^(2*n-3) * (2^n + 12) */
}
