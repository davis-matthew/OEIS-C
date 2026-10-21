/*
 * A203154  (n-1)-st elementary symmetric function of {2, 3, 3, 4, 4, 5,
 *          5,...,Floor[(n+4)/2]}.
 * https://oeis.org/A203154
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{i=1}^n ∏_{j≠i} (⌊j/2⌋ + 2)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 22, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This equals the (n‑1)-th elementary symmetric function of
   {2,3,3,4,4,5,5,…,⌊(n+4)/2⌋}. */

#include <stdint.h>

seq_t a(idx_t n) {
    seq_t prod = 1;                 /* product of all terms s[1..n]   */
    idx_t i;

    /* Build the product ∏_{i=1}^n (⌊i/2⌋ + 2) */
    for (i = 1; i <= n; ++i) {
        seq_t si = (seq_t)((i >> 1) + 2);   /* floor(i/2)+2  */
        prod *= si;
    }

    /* Sum over prod / s[i]  =  ∑ (product of all terms except s[i]) */
    seq_t sum = 0;
    for (i = 1; i <= n; ++i) {
        seq_t si = (seq_t)((i >> 1) + 2);
        sum += prod / si;
    }

    return sum;
}
