/*
 * A161007  a(n+1) = 2*a(n) + 16*a(n-1), a(0)=0, a(1)=1.
 * https://oeis.org/A161007
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) + 16*a(n-2), a(0)=0, a(1)=1
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 14, u64 n <= 28, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t */

/* Computes the n-th term of OEIS sequence A161007. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (n == 1) return 1;

    seq_t prev2 = 0;   /* a(0) */
    seq_t prev1 = 1;   /* a(1) */
    seq_t cur;

    for (idx_t i = 2; i <= n; ++i) {
        cur = prev1 * 2 + prev2 * 16;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
