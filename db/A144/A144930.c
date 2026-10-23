/*
 * A144930  Numbers k arising in A144929.
 * https://oeis.org/A144930
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 10, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A144930: Numbers k arising in A144929.
   a(1) = 1, a(2) = 109,
   a(n) = 110*a(n-1) - a(n-2) for n >= 3 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Defensive handling of non‑positive indices */
    if (n <= 0)
        return (seq_t)0;

    if (n == 1)
        return (seq_t)1;

    if (n == 2)
        return (seq_t)109;

    seq_t prev2 = (seq_t)1;   /* a(1) */
    seq_t prev1 = (seq_t)109; /* a(2) */
    seq_t cur;

    for (idx_t i = 3; i <= n; ++i) {
        cur   = (prev1 * (seq_t)110) - prev2;
        prev2 = prev1;
        prev1 = cur;
    }

    return prev1; /* a(n) for n >= 3 */
}
