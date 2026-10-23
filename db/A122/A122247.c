/*
 * A122247  Partial sums of A005187.
 * https://oeis.org/A122247
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Sum_{k=1..n} Sum_{j=0..n} floor(k/2^j) Equivalent: a(n) =
 *            n*(n+1) - Σ_{k=1..n} popcount(k)
 * Range:     u8 n <= 16, u16 n <= 257, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   popcount_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/popcount_sum.h"

seq_t a(idx_t n)
{
    /* n*(n+1) fits into seq_t (arbitrary precision if necessary). */
    seq_t total = (seq_t)n * ((seq_t)n + 1);
    seq_t popsum = popcount_sum(n);
    return total - popsum;
}
