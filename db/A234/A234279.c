/*
 * A234279  Number of spanning forests of a benzenoid chain of length n.
 * https://oeis.org/A234279
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 32*a(n-1) - 16*a(n-2) (n ≥ 2) a(0) = 2 a(1) = 63 (OEIS
 *            A234279)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>


seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)2;
    if (n == 1) return (seq_t)63;

    seq_t prev2 = (seq_t)2;   /* a(n-2) */
    seq_t prev1 = (seq_t)63;  /* a(n-1) */
    seq_t cur;

    for (idx_t i = 2; i <= n; ++i) {
        cur = (seq_t)((seq_t)32 * prev1 - (seq_t)16 * prev2);
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
