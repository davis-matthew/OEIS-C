/*
 * A122249  Numerators of Hankel transform of 1/(2n+1).
 * https://oeis.org/A122249
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 4^e(n), where e(n) = n*(n+1) - sum_{k=1}^n popcount(k)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 5, u128 n <= 8,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation uses the identity sum_{j>=0} floor(k/2^j) = 2k - popcount(k) */

#include <stdint.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Sum of popcounts of 1..n */
    seq_t pop_sum = 0;
    for (idx_t k = 1; k <= n; ++k)
        pop_sum += (seq_t)__builtin_popcountll(k);

    /* e = n*(n+1) - pop_sum */
    seq_t nseq = (seq_t)n;
    seq_t prod = nseq * ((seq_t)n + 1);   /* n*(n+1) */
    seq_t e = prod - pop_sum;

    /* a(n) = pow2( 2*e )  because 4^e = 2^(2e) */
    idx_t exp = 2 * (idx_t)e;   /* exponent fits in idx_t for n up to ~1e5 */
    return pow2(exp);
}
