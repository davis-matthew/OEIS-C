/*
 * A190154  Diagonal sums of the triangle A190152.
 * https://oeis.org/A190154
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..floor(n/2)} binomial(3*n-4*k, 3*n-6*k)
 *            Recurrence derived from the generating function: a(n) = 2*a(n-1)
 *            + 2*a(n-2) + 6*a(n-3) - 3*a(n-4) + a(n-6) for n ≥ 6 Initial
 *            values: a(0)=1, a(1)=1, a(2)=2, a(3)=11, a(4)=30, a(5)=91
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 39, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* For the first six indices the values are small constants. */
    if (n <= 5)
    {
        switch ((unsigned)n) {
            case 0: return (seq_t)1;
            case 1: return (seq_t)1;
            case 2: return (seq_t)2;
            case 3: return (seq_t)11;
            case 4: return (seq_t)30;
            case 5: return (seq_t)91;
        }
    }

    /* Allocate a temporary array to store the computed sequence up to n. */
    seq_t *buf = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    /* Base case values. */
    buf[0] = (seq_t)1;
    buf[1] = (seq_t)1;
    buf[2] = (seq_t)2;
    buf[3] = (seq_t)11;
    buf[4] = (seq_t)30;
    buf[5] = (seq_t)91;

    /* Compute the remaining terms using the recurrence. */
    for (idx_t i = 6; i <= n; ++i)
    {
        seq_t temp = buf[i-6];                 /* a(n-6)   */
        temp += (seq_t)2 * buf[i-1];           /* 2*a(n-1) */
        temp += (seq_t)2 * buf[i-2];           /* 2*a(n-2) */
        temp += (seq_t)6 * buf[i-3];           /* 6*a(n-3) */
        /* Subtract 3*a(n-4) after the summation to avoid underflow */
        temp -= (seq_t)3 * buf[i-4];           /* -3*a(n-4)*/
        buf[i] = temp;                         /* a(n)     */
    }

    seq_t result = buf[n];
    free(buf);
    return result;
}
