/*
 * A025040  Number of partitions of { 1, 2, ..., 8n } into sets of size 8.
 * https://oeis.org/A025040
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (8n)!/(n!(8!)^n) = Π_{k=1}^n binom(8k,8)/k
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 2, u64 n <= 4, u128 n <= 6,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation builds the answer multiplicatively, using the
   provided binom(a,b) helper. No memoisation or external libraries are
   required beyond the usual math utilities available in the build. */

#include "../utils/binom.h"   /* provides seq_t binom(idx_t a, idx_t b) */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    seq_t result = (seq_t)1;
    for (idx_t k = 1; k <= n; ++k)
    {
        seq_t b = binom(8 * k, (idx_t)8);
        result = result * b / (seq_t)k;   /* exact division */
    }
    return result;
}
