/*
 * A099583  a(n) = Sum_{k=0..floor(n/2)} binomial(n-k,
 *          k-1)*2^(n-k-1)*(3/2)^(k-1).
 * https://oeis.org/A099583
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2*a(n-1) + 6*a(n-2) - 6*a(n-3) - 9*a(n-4); Initial
 *            values: a(0)=0, a(1)=0, a(2)=1, a(3)=2. Uses a simple O(n)
 *            recurrence that needs only the last four terms
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 22, u64 n <= 42, u128 n <= 82,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Handle the first four terms directly */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)1;
    if (n == 3) return (seq_t)2;

    /* a0,a1,a2,a3 hold the last four computed values:
           a0 = a(n-4), a1 = a(n-3), a2 = a(n-2), a3 = a(n-1).  */
    seq_t a0 = (seq_t)0;   /* a(0)   */
    seq_t a1 = (seq_t)0;   /* a(1)   */
    seq_t a2 = (seq_t)1;   /* a(2)   */
    seq_t a3 = (seq_t)2;   /* a(3)   */
    seq_t new_val;

    /* Iterate from 4 up to n, updating the sliding window each time. */
    for (idx_t i = 4; i <= n; ++i) {
        /* Compute a(i) using the recurrence. The expression is safe
           because 2*a(i-1) + 6*a(i-2) ≥ 6*a(i-3) + 9*a(i-4). */
        new_val = 2 * a3 + 6 * a2 - 6 * a1 - 9 * a0;

        /* Shift window: a0 = a1, a1 = a2, a2 = a3, a3 = new_val. */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = new_val;
    }

    return a3;   /* a(n) is now stored in a3 */
}
