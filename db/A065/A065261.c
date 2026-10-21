/*
 * A065261  The siteswap sequence (the deltas p[i]-i, i in ]-inf,+inf[, folded
 *          from Z to N, mapping 0->1, 1->2, -1->3, 2->4, -2->5, etc.) for
 *          A065260.
 * https://oeis.org/A065261
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n/2 for even n; a(n) = n if n ≡ 1 (mod 4); a(n) = (n+1)/4
 *            if n ≡ 3 (mod 4)
 * Range:     u8 n <= 76, u16 n <= 76, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements OEIS A065261 – the siteswap sequence. */
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                /* undefined for n = 0, return 0 */

    /* Even indices: a(2k) = k */
    if ((n & 1ULL) == 0)
        return (seq_t)(n >> 1);

    /* Odd indices */
    if ((n & 3ULL) == 1)          /* n ≡ 1 (mod 4) */
        return (seq_t)n;
    /* Remaining case: n ≡ 3 (mod 4) */
    return (seq_t)((n >> 2) + 1);
}
