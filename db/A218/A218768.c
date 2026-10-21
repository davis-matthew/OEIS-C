/*
 * A218768  a(n+2) = (2*n+1)^2*a(n+1) + (2*n+1)*(2*n-1)*a(n) with a(1)=1 and
 *          a(2)=2.
 * https://oeis.org/A218768
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(1) = 1, a(2) = 2; for n > 2: a(n) = (2*n-3)^2 * a(n-1) +
 *            (2*n-3)*(2*n-5) * a(n-2)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 7, u64 n <= 11, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

extern seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)1;          /* a(1) = 1 */
    if (n == 2) return (seq_t)2;          /* a(2) = 2 */

    /* use only two previous values to avoid large allocations */
    seq_t a_prev2 = (seq_t)1;             /* a(1) */
    seq_t a_prev1 = (seq_t)2;             /* a(2) */
    seq_t a_curr;

    for (idx_t k = 3; k <= n; ++k) {
        seq_t t = (seq_t)(2 * k - 3);                 /* (2*k-3) */
        seq_t t1 = t * t;                            /* (2*k-3)^2 */
        seq_t t2 = t * (seq_t)(2 * k - 5);           /* (2*k-3)*(2*k-5) */
        a_curr = t1 * a_prev1 + t2 * a_prev2;
        a_prev2 = a_prev1;
        a_prev1 = a_curr;
    }
    return a_prev1;
}
