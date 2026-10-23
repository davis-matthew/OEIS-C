/*
 * A139593  A139276(n) followed by A139272(n+1).
 * https://oeis.org/A139593
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabf
 * Formula:   a(n) = (-1 + (-1)^n + 6*n + 8*n^2)/4
 * Range:     u8 n <= 5, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  a(0)=0, a(1)=3, a(2)=11, a(3)=22, ...  (OEIS A139593) */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Convert index to seq_t for arithmetic */
    seq_t m   = (seq_t)n;
    seq_t sq  = m * m;                     /* n^2 */
    seq_t num = (seq_t)6 * m + (seq_t)8 * sq;  /* 6*n + 8*n^2 */

    /* Add the (-1 + (-1)^n) part:
       if n is odd: subtract 2, else add 0 */
    if (n & 1U) {
        num -= (seq_t)2;
    }

    /* Divide by 4 (the numerator is always divisible by 4) */
    return num / (seq_t)4;
}
