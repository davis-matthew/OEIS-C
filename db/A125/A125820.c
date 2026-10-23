/*
 * A125820  a(n) = ((1 + 7*sqrt(2))^n + (1 - 7*sqrt(2))^n)/2.
 * https://oeis.org/A125820
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS sequence A125820:
   a(1) = 1, a(2) = 1
   a(n) = 2*a(n-1) + 97*a(n-2)   for n >= 3
   (With a(0) = 1 for the recurrence.)
   Offset: 1
*/
#include <stdint.h>

seq_t a(idx_t n)
{
    /* handle the offset terms */
    if (n <= 2) {
        return (seq_t)1;
    }

    /* a(0) and a(1) are both 1 */
    seq_t prev2 = (seq_t)1;   /* a(n-2) */
    seq_t prev1 = (seq_t)1;   /* a(n-1) */
    seq_t curr  = 0;

    /* iterate from n = 3 up to the requested n */
    for (idx_t i = 3; i <= n; ++i) {
        curr = (seq_t)2 * prev1 + (seq_t)97 * prev2;
        prev2 = prev1;
        prev1 = curr;
    }

    return prev1;
}
