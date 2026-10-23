/*
 * A192924  Constant term in the reduction by (x^2->x+1) of the polynomial
 *          p(n,x) defined below at Comments.
 * https://oeis.org/A192924
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n)+2*(-n+1)*a(n-1)+(n^2-3*n-1)*a(n-2)+3*(n-2)*a(n-3)+a(n-4)=0
 *            Initial values: a(0)=1, a(1)=0, a(2)=1, a(3)=4
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle the first four terms explicitly. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)1;
    if (n == 3) return (seq_t)4;

    /* v0 = a(n-4), v1 = a(n-3), v2 = a(n-2), v3 = a(n-1) */
    seq_t v0 = (seq_t)1;          /* a(0)   */
    seq_t v1 = (seq_t)0;          /* a(1)   */
    seq_t v2 = (seq_t)1;          /* a(2)   */
    seq_t v3 = (seq_t)4;          /* a(3)   */

    for (idx_t i = 4; i <= n; ++i)
    {
        /* Compute a(i) using the recurrence:
           a(i) = 2*(i-1)*a(i-1) - (i^2-3*i-1)*a(i-2) - 3*(i-2)*a(i-3) - a(i-4) */
        seq_t term1 = (seq_t)(2 * i - 2) * v3;
        seq_t term2 = (seq_t)(i * i - 3 * i - 1) * v2;
        seq_t term3 = (seq_t)3 * (i - 2) * v1;
        seq_t term4 = v0;  /* a(i-4) */
        seq_t ai = term1 - term2 - term3 - term4;

        /* Slide the window for the next iteration. */
        v0 = v1;
        v1 = v2;
        v2 = v3;
        v3 = ai;
    }

    return v3;   /* v3 now holds a(n) */
}
