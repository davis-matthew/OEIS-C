/*
 * A087088  Positive ruler-type fractal sequence with 1's in every third
 *          position.
 * https://oeis.org/A087088
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, look
 * Formula:   a(n) = 1 if n == 1 (mod 3), otherwise a(n) = a(n-ceiling(n/3)) +
 *            1. This implementation evaluates the recurrence iteratively, so
 *            no dynamic allocation or memoisation is required
 * Range:     u8 n <= 9447, u16 n <= 9447, u32 n <= 9447, u64 n <= 9447,
 *            u128 n <= 9447, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 1ULL)
        abort();                       /* the sequence is defined for n >= 1 */

    seq_t ans = 0;                     /* number of recursion steps taken */

    for (;;) {
        idx_t r = n % 3ULL;
        if (r == 1ULL)
            return ans + 1;             /* a terminating 1 has been reached */

        /* otherwise apply the recurrence step */
        idx_t sub = (n + 2ULL) / 3ULL;   /* ceiling of n/3 */
        n -= sub;                       /* new index */
        ++ans;                           /* increment step count */
    }
}
