/*
 * A079935  a(n) = 4*a(n-1) - a(n-2) with a(1) = 1, a(2) = 3.
 * https://oeis.org/A079935
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1) - a(n-2); a(1) = 1, a(2) = 3
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 18, u64 n <= 34, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Computes the n-th term of OEIS sequence A079935 (1‑based). */
seq_t a(idx_t n)
{
    if (n == 1) {
        return (seq_t)1;
    }
    if (n == 2) {
        return (seq_t)3;
    }

    seq_t prev2 = (seq_t)1;  /* a(1) */
    seq_t prev1 = (seq_t)3;  /* a(2) */
    seq_t cur;

    for (idx_t i = 3; i <= n; ++i) {
        cur = (seq_t)4 * prev1 - prev2;  /* recurrence */
        prev2 = prev1;
        prev1 = cur;
    }

    return prev1;
}
