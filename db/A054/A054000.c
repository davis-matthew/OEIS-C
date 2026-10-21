/*
 * A054000  a(n) = 2*n^2 - 2.
 * https://oeis.org/A054000
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*n^2 - 2
 * Range:     u8 n <= 11, u16 n <= 181, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert the index to the sequence type. */
    seq_t nn = (seq_t)n;
    /* Compute n^2. */
    seq_t sq = nn * nn;
    /* Compute 2*n^2. */
    seq_t val = sq * 2;
    /* Subtract 2. */
    val = val - 2;
    return val;
}
