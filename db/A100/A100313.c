/*
 * A100313  Number of 4 X n binary matrices avoiding simultaneously the right
 *          angled numbered polyomino patterns (ranpp) (10;0) and (01;1).
 * https://oeis.org/A100313
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1/3)*n*(n^2 + 9*n + 14)*2^n for n>0; a(0)=1
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 20, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n) {
    if (n == (idx_t)0) {
        return (seq_t)1;
    }
    seq_t nn = (seq_t)n;
    seq_t term = nn * nn;      /* n^2 */
    term += (seq_t)9 * nn;     /* n^2 + 9n */
    term += (seq_t)14;         /* n^2 + 9n + 14 */
    term *= nn;                /* n*(n^2 + 9n + 14) */
    term /= (seq_t)3;          /* divide by 3 */
    seq_t power = pow2(n);     /* 2^n */
    return term * power;
}
