/*
 * A194482  Number of ways to arrange 5 indistinguishable points on an n X n X
 *          n triangular grid so that no four points are in the same row or
 *          diagonal.
 * https://oeis.org/A194482
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n^10 +5*n^9 -10*n^8 -118*n^7 +281*n^6 +149*n^5 -800*n^4
 *            +348*n^3 +528*n^2 -384*n)/3840
 * Range:     u8 none, u16 n <= 3, u32 n <= 8, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Convert n to the sequence type */
    seq_t tn = (seq_t)n;

    /* Compute powers of n up to n^10 */
    seq_t pow[11];
    pow[0] = (seq_t)1;
    pow[1] = tn;
    for (idx_t i = 2; i <= 10; ++i)
        pow[i] = pow[i-1] * tn;

    /* Sum of positive coefficient terms */
    seq_t sum_pos =
        pow[10]                            /* n^10 */
      + (seq_t)5 * pow[9]                  /* 5*n^9 */
      + (seq_t)281 * pow[6]                /* 281*n^6 */
      + (seq_t)149 * pow[5]                /* 149*n^5 */
      + (seq_t)348 * pow[3]                /* 348*n^3 */
      + (seq_t)528 * pow[2];               /* 528*n^2 */

    /* Sum of negative coefficient terms */
    seq_t sum_neg =
        (seq_t)10 * pow[8]                 /* 10*n^8 */
      + (seq_t)118 * pow[7]                /* 118*n^7 */
      + (seq_t)800 * pow[4]                /* 800*n^4 */
      + (seq_t)384 * pow[1];               /* 384*n */

    /* Compute the numerator (difference) */
    seq_t num = sum_pos - sum_neg;  /* exact division guaranteed */

    /* Final value */
    return num / (seq_t)3840;
}
