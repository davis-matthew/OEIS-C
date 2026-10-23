/*
 * A130876  Centered 1729-gonal numbers.
 * https://oeis.org/A130876
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 + (1729 * n * (n + 1)) / 2
 * Range:     u8 n <= 0, u16 n <= 8, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t tri = nn * (nn + 1) / 2;   /* triangular number: n*(n+1)/2 */
    return 1 + (seq_t)1729 * tri;
}
