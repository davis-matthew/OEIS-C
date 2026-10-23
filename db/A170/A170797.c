/*
 * A170797  a(n) = n^10*(n^5+1)/2.
 * https://oeis.org/A170797
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^10 * (n^5 + 1) / 2
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 20, u128 n <= 388,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle trivial cases */
    if (n <= 1) return (seq_t)n;   /* a(0)=0, a(1)=1 */

    seq_t x = (seq_t)n;

    /* Compute n^5  (safe for the required range of n) */
    seq_t n5 = x * x * x * x * x;

    /* Avoid overflow by dividing by 2 on the even factor
       (n^10 is even iff n is even; n^5+1 is even iff n is odd) */
    if ((n & 1) == 0) {
        /* n even: divide n^5 by 2 first */
        seq_t n5_half = n5 >> 1;           /* n^5 / 2          */
        seq_t n10_half = n5_half * n5;     /* (n^5/2) * n^5  */
        seq_t res = n10_half * (n5 + 1);   /* (n^10/2) * (n^5+1) */
        return res;
    } else {
        /* n odd: divide (n^5+1) by 2 first */
        seq_t b_half = (n5 + 1) >> 1;      /* (n^5+1)/2       */
        seq_t n10 = n5 * n5;               /* n^10            */
        seq_t res = n10 * b_half;          /* n^10 * ((n^5+1)/2) */
        return res;
    }
}
