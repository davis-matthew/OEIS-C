/*
 * A020957  a(n) = Sum_{k >= 1} floor(2*tau^(n-k)).
 * https://oeis.org/A020957
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1/4)*(8*Lucas(n+1) - 2*n -5 + (-1)^n) (n > 1); a(1)=3
 * Range:     u8 n <= 6, u16 n <= 17, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* undefined for offset 1, but safe guard */
    if (n == 1) return (seq_t)3;

    /* Compute Lucas(n+1) iteratively */
    seq_t L0 = (seq_t)2;           /* Lucas(0)  */
    seq_t L1 = (seq_t)1;           /* Lucas(1)  */
    for (idx_t i = 2; i <= n + 1; ++i) {
        seq_t L2 = L0 + L1;
        L0 = L1;
        L1 = L2;
    }
    seq_t lucas = L1;              /* Lucas(n+1) */

    seq_t n_seq = (seq_t)n;
    seq_t num = (seq_t)8 * lucas;   /* numerator of the formula */
    num -= (seq_t)2 * n_seq;
    num -= (seq_t)5;
    if ((n & (idx_t)1) != 0)       /* (-1)^n = -1 if n is odd, +1 if even */
        num -= (seq_t)1;
    else
        num += (seq_t)1;

    return num / (seq_t)4;          /* division truncates towards zero */
}
