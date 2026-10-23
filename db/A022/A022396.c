/*
 * A022396  Fibonacci sequence beginning 1, 26.
 * https://oeis.org/A022396
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(n-2), a(0) = 1, a(1) = 26
 * Range:     u8 n <= 6, u16 n <= 17, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == (idx_t)0)
        return (seq_t)1;
    if (n == (idx_t)1)
        return (seq_t)26;

    seq_t prev2 = (seq_t)1;  /* a(0) */
    seq_t prev1 = (seq_t)26; /* a(1) */
    seq_t cur;

    for (idx_t i = (idx_t)2; ; ++i) {
        cur = prev1 + prev2;
        if (i == n)
            return cur;
        prev2 = prev1;
        prev1 = cur;
    }
}
