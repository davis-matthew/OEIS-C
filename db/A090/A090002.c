/*
 * A090002  Length of longest contiguous block of 1's in binary expansion of
 *          n-th triangular number.
 * https://oeis.org/A090002
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = length of longest contiguous block of 1's in binary
 *            representation of n*(n+1)/2
 * Range:     u8 n <= 29, u16 n <= 2016, u32 n <= 9979, u64 n <= 9979,
 *            u128 n <= 9979, big any n
 * Helpers:   longest_block_of_ones
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/longest_block_of_ones.h"

seq_t a(idx_t n)
{
    seq_t triangular = (seq_t)n * ((seq_t)n + 1) / 2;
    return longest_block_of_ones(triangular);
}
