/*
 * A184115  n + ceiling(n^2/25); complement of A184114.
 * https://oeis.org/A184115
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n + ceil(n^2 / 25)
 * Range:     u8 n <= 15, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert the index to the sequence type before any arithmetic. */
    seq_t N = (seq_t)n;

    /* Compute the square of the index.  For seq_t types that are larger
       than 64 bits (or arbitrary-precision) this is safe; for 64-bit
       types we rely on the final result fitting into seq_t. */
    seq_t sq = N * N;

    /* Ceiling division by 25: ceil(x/25) = (x + 24) / 25 for positive x. */
    seq_t ceil_div = (sq + (seq_t)24) / (seq_t)25;

    return N + ceil_div;
}
