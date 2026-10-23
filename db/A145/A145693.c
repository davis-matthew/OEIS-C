/*
 * A145693  Numbers X such that there exists Y in N with X^2=21*Y^2+7.
 * https://oeis.org/A145693
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 9, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A145693
   Recurrence: a(1) = 14, a(2) = 1526,  a(n) = 110*a(n-1) – a(n-2)  (n ≥ 3)
*/
#include <stddef.h>

seq_t a(idx_t n)
{
    /*  The sequence is defined for n ≥ 1.  Calling with n < 1 is undefined
        and we simply return 0. */
    if (n < 1) return (seq_t)0;

    /* Handle the first two terms directly */
    if (n == 1) return (seq_t)14;
    if (n == 2) return (seq_t)1526;

    /* Compute iteratively from a(1) up to a(n).
       Two variables enough because the recurrence only needs the two
       previous values. */
    seq_t prev2 = (seq_t)14;    /* a(1)   */
    seq_t prev1 = (seq_t)1526;  /* a(2)   */

    for (idx_t i = 3; i <= n; ++i) {
        seq_t cur = (seq_t)110 * prev1 - prev2;  /* a(i)   */
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
