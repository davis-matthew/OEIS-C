/*
 * A090002  Length of longest contiguous block of 1's in binary expansion of
 *          n-th triangular number.
 * https://oeis.org/A090002
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = length of longest contiguous block of 1's in binary
 *            representation of n*(n+1)/2
 * Range:     u8 n <= 29, u16 n <= 2016, u32 n <= 16384, u64 n <= 16384,
 *            u128 n <= 16384, big any n
 * Validated: standard, pass, 118 terms through n = 9979 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t longest_block_of_ones(seq_t x)
{
    seq_t maxrun = 0, cur = 0;
    while (x != 0) {
        if ((x & 1) != 0) {          /* lowest bit is 1 */
            cur += 1;
            if (cur > maxrun) maxrun = cur;
        } else {
            cur = 0;
        }
        x >>= 1;
    }
    return maxrun;
}

seq_t a(idx_t n)
{
    seq_t triangular = (seq_t)n * ((seq_t)n + 1) / 2;
    return longest_block_of_ones(triangular);
}
