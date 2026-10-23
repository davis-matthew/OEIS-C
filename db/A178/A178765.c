/*
 * A178765  a(n) = 17*a(n-1) + a(n-2), with a(-1) = 0 and a(0) = 1.
 * https://oeis.org/A178765
 *
 * Offset:    -1    (a(-1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 17*a(n-1) + a(n-2), a(-1) = 0, a(0) = 1
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Compute the nᵗʰ term of A178765.  The recurrence is evaluated
   iteratively; no memoisation is required for n up to about 10 000. */
seq_t a(idx_t n)
{
    if (n < 0) {
        /* offset is –1, below the offset behaviour is irrelevant */
        return 0;
    }
    if (n == 0) {
        return 1;
    }
    seq_t prev2 = 0;   /* a(n-2)  */
    seq_t prev1 = 1;   /* a(n-1)  */
    seq_t cur   = 0;   /* a(n)    */
    for (idx_t i = 1; i <= n; ++i) {
        cur = 17 * prev1 + prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return cur;
}
