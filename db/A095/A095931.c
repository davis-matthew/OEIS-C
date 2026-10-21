/*
 * A095931  Number of walks of length 2n between two nodes at distance 4 in
 *          the cycle graph C_10.
 * https://oeis.org/A095931
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(2) = 1, a(3) = 7, a(4) = 36 a(n) = 7*a(n-1) - 13*a(n-2) +
 *            4*a(n-3) for n ≥ 5 This is a linear recurrence with constant
 *            coefficients. The implementation computes the recurrence
 *            directly, which is O(n) time per call and works for any order of
 *            calls. It uses only integer arithmetic on type seq_t,...
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 33, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Return 0 for indices before the offset (allowed, behavior undefined). */
    if (n < 2) return 0;

    /* Handle the three initial values directly. */
    if (n == 2) return (seq_t)1;
    if (n == 3) return (seq_t)7;
    if (n == 4) return (seq_t)36;

    /* For n ≥ 5, iterate the recurrence. */
    seq_t prev3 = (seq_t)1; /* a(2)   */
    seq_t prev2 = (seq_t)7; /* a(3)   */
    seq_t prev1 = (seq_t)36;/* a(4)   */
    seq_t curr = 0;

    for (idx_t i = 5; i <= n; ++i) {
        curr = ((seq_t)7) * prev1
             - ((seq_t)13) * prev2
             + ((seq_t)4) * prev3;
        /* Shift the window for next iteration. */
        prev3 = prev2;
        prev2 = prev1;
        prev1 = curr;
    }

    return curr;
}
