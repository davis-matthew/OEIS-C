/*
 * A121079  a(n) = Sum_{i=0..n} C(n,i)^2*i!*4^i + 2^n*n!.
 * https://oeis.org/A121079
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{i=0..n} C(n,i)^2 * i! * 4^i + 2^n * n!
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t sum = 0;
    seq_t binom = 1;   /* C(n,0) */
    seq_t fact = 1;    /* 0! */
    seq_t pow4 = 1;    /* 4^0 */

    for (idx_t i = 0; i <= n; ++i) {
        /* term = binom^2 * fact * pow4 */
        seq_t term = binom * binom;
        term = term * fact;
        term = term * pow4;
        sum += term;

        if (i < n) {
            /* update binom to C(n,i+1) */
            binom = binom * (seq_t)(n - i);
            binom = binom / (seq_t)(i + 1);
            /* update factorial */
            fact = fact * (seq_t)(i + 1);
            /* update 4^i */
            pow4 = pow4 * 4;
        }
    }

    /* add the 2^n * n! term */
    seq_t pow2 = 1;
    for (idx_t i = 0; i < n; ++i) {
        pow2 = pow2 * 2;
    }
    sum += pow2 * fact;   /* fact now holds n! after the loop */

    return sum;
}
