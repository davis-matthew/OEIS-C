/*
 * A230900  a(n) = 2^Lucas(n).
 * https://oeis.org/A230900
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n‑1) * a(n‑2) with a(0) = 4, a(1) = 2 (OEIS A230900)
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 7, u64 n <= 8, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Handle the two initial terms directly. */
    if (n == 0) return (seq_t)4;
    if (n == 1) return (seq_t)2;

    /* Iteratively compute the recurrence.
       prev2 holds a(i‑2), prev1 holds a(i‑1). */
    seq_t prev2 = (seq_t)4;   /* a(0) */
    seq_t prev1 = (seq_t)2;   /* a(1) */
    seq_t cur;

    for (idx_t i = 2; i <= n; ++i) {
        cur   = prev1 * prev2;   /* a(i) = a(i‑1) * a(i‑2) */
        prev2 = prev1;           /* shift for next iteration */
        prev1 = cur;
    }
    return cur;
}
