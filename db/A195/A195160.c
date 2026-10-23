/*
 * A195160  Generalized 11-gonal (or hendecagonal) numbers: m*(9*m - 7)/2 with
 *          m = 0, 1, -1, 2, -2, 3, -3, ...
 * https://oeis.org/A195160
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (18*n*(n+1)+5*(2*n+1)*(-1)^n-5)/16
 * Range:     u8 n <= 3, u16 n <= 137, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;                     /* convert index to seq_t  */
    seq_t prod = sn * (sn + 1);              /* n*(n+1)                    */
    seq_t term18 = prod * (seq_t)18;         /* 18*n*(n+1)                 */
    seq_t term5base = sn * (seq_t)2 + (seq_t)1; /* 2*n+1                     */
    seq_t term5 = term5base * (seq_t)5;      /* 5*(2*n+1)                  */
    seq_t num;

    if ((n & (idx_t)1) == 0) {               /* n even */
        num = term18 + term5 - (seq_t)5;
    } else {                                 /* n odd  */
        num = term18 - term5 - (seq_t)5;
    }
    return num / (seq_t)16;
}
