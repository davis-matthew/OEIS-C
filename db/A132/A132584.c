/*
 * A132584  a(0)=0, a(1)=4; for n > 1, a(n) = 18*a(n-1) - a(n-2) + 8.
 * https://oeis.org/A132584
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 15, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A132584
 * a(0) = 0
 * a(1) = 4
 * a(n) = 18*a(n-1) - a(n-2) + 8   for n > 1
 */
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Handle the first two terms directly */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)4;

    /* Iteratively compute using the recurrence */
    seq_t prev2 = (seq_t)0;   /* a(n-2)  */
    seq_t prev1 = (seq_t)4;   /* a(n-1)  */
    seq_t cur   = (seq_t)0;   /* a(n)    */

    for (idx_t i = 2; i <= n; ++i) {
        cur   = (seq_t)18 * prev1 - prev2 + (seq_t)8;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
