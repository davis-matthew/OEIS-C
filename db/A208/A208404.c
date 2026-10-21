/*
 * A208404  Number of n X 4 0..3 arrays with new values 0..3 introduced in row
 *          major order and no element equal to more than two of its immediate
 *          leftward or upward or right-upward antidiagonal neighbors.
 * https://oeis.org/A208404
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 8, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 262*a(n-1) - 3385*a(n-2) + 2868*a(n-3) - 576*a(n-4)
   Initial values: a(1)=15, a(2)=2698, a(3)=658450, a(4)=163422914, a(5)=40595679634
*/
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* The sequence is only defined for n >= 1 */
    if (n == 0) return (seq_t)0;

    /* Known values for n = 1..5 */
    if (n == 1) return (seq_t)15;
    if (n == 2) return (seq_t)2698;
    if (n == 3) return (seq_t)658450;
    if (n == 4) return (seq_t)163422914;
    if (n == 5) return (seq_t)40595679634ULL;

    /* Recurrence for n >= 6.  Keep the last four values only. */
    seq_t prev4 = (seq_t)2698;      /* a(2)  */
    seq_t prev3 = (seq_t)658450;    /* a(3)  */
    seq_t prev2 = (seq_t)163422914; /* a(4)  */
    seq_t prev1 = (seq_t)40595679634ULL; /* a(5) */

    seq_t current = 0;
    for (idx_t i = 6; i <= n; ++i) {
        current = (seq_t)262 * prev1
                - (seq_t)3385 * prev2
                + (seq_t)2868 * prev3
                - (seq_t)576  * prev4;

        /* Shift the window of previous values */
        prev4 = prev3;
        prev3 = prev2;
        prev2 = prev1;
        prev1 = current;
    }
    return current;
}
