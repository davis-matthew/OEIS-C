/*
 * A194124  a(n) = Sum_{k=0..floor(n/2)} (C(n+2, k+2) - C(n+2, k)).
 * https://oeis.org/A194124
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/2)} (C(n+2, k+2) - C(n+2, k))
 * Range:     u8 n <= 5, u16 n <= 13, u32 n <= 28, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    idx_t m = n >> 1;             /* floor(n/2)           */
    idx_t N = n + 2;              /* n+2                   */
    seq_t term1 = binom(N, m + 1);/* C(n+2, floor(n/2)+1) */
    seq_t term2 = binom(N, m + 2);/* C(n+2, floor(n/2)+2) */
    seq_t sum   = term1 + term2;  /* sum of the two binomials */
    seq_t ans   = sum - ((seq_t)n + (seq_t)3); /* subtract n+3 (=1+N) */
    return ans;
}
