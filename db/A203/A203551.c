/*
 * A203551  a(n) = n*(5n^2 + 3n + 4) / 6.
 * https://oeis.org/A203551
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(5n^2 + 3n + 4) / 6
 * Range:     u8 n <= 3, u16 n <= 23, u32 n <= 1389, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* convert the index to the algorithm’s sized integer type */
    seq_t nn = (seq_t)n;
    seq_t num = nn * (5 * nn * nn + 3 * nn + 4);
    return num / 6;
}
