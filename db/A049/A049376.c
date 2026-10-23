/*
 * A049376  Row sums of triangle A046089.
 * https://oeis.org/A049376
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n+3) = (3*n+7)*a(n+2) - 3*(n+1)*(n+2)*a(n+1) +
 *            n*(n+1)*(n+2)*a(n) Initial values: a(0)=1, a(1)=1, a(2)=4
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 18, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)1;
    if (n == (idx_t)1) return (seq_t)1;
    if (n == (idx_t)2) return (seq_t)4;

    /* Keep the last three terms of the sequence only. */
    seq_t a0 = (seq_t)1;  /* a(0)   */
    seq_t a1 = (seq_t)1;  /* a(1)   */
    seq_t a2 = (seq_t)4;  /* a(2)   */

    for (idx_t i = 3; i <= n; ++i) {
        idx_t k = i - 3;                           /* n in the recurrence */
        seq_t term1 = a2 * (seq_t)(3*k + 7);        /* (3*n+7)*a(n+2) */
        seq_t term2 = a1 * (seq_t)(3*(k+1)*(k+2));  /* 3*(n+1)*(n+2)*a(n+1) */
        seq_t term3 = a0 * (seq_t)(k*(k+1)*(k+2));  /* n*(n+1)*(n+2)*a(n) */
        seq_t ai = term1 + term3 - term2;           /* a(n+3) */

        /* Shift the window for the next iteration. */
        a0 = a1;
        a1 = a2;
        a2 = ai;
    }

    return a2;  /* after the loop, a2 holds a(n) */
}
