/*
 * A014831  a(1)=1; for n>1, a(n) = 8*a(n-1) + n.
 * https://oeis.org/A014831
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1) = 1; for n > 1, a(n) = 8 * a(n-1) + n (equivalent to a(n) =
 *            (8^(n+1) - 7*n - 8) / 49) This implementation uses the
 *            recurrence, which is O(n) and works for any seq_t that supports
 *            the basic arithmetic operations
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 22, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == (idx_t)1)
        return (seq_t)1;

    seq_t val = (seq_t)1;               /* a(1) */
    idx_t i;
    for (i = (idx_t)2; i <= n; ++i) {
        val = val * (seq_t)8 + (seq_t)i;
    }
    return val;
}
