/*
 * A018904  Define the sequence S(a(0),a(1)) by a(n+2) is the least integer
 *          such that a(n+2)/a(n+1) > a(n+1)/a(n) for n >= 0. This is S(1,6).
 * https://oeis.org/A018904
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 24, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = 7*a(n-1) - 5*a(n-2)   for n >= 2
 *          a(0) = 1
 *          a(1) = 6
 */
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Handle the base cases directly. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)6;

    /* Use two scratch variables to keep the last two values. */
    seq_t prev2 = (seq_t)1;   /* a(n-2) */
    seq_t prev1 = (seq_t)6;   /* a(n-1) */
    seq_t cur   = (seq_t)0;   /* a(n)   */

    /* Simple O(n) recurrence – works for all supported seq_t sizes. */
    for (idx_t i = 2; i <= n; ++i) {
        /* Since seq_t is unsigned, the subtraction is well‑defined because
           the sequence guarantees a(n-1) > a(n-2), hence 7*a(n-1) > 5*a(n-2). */
        cur = (7 * prev1) - (5 * prev2);
        prev2 = prev1;
        prev1 = cur;
    }

    return cur;
}
