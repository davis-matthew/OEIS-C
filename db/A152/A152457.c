/*
 * A152457  Partial sums of A027444.
 * https://oeis.org/A152457
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n(n + 1)(3n^2 + 7n + 8)/12
 * Range:     u8 n <= 2, u16 n <= 11, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t n0 = (seq_t)n;
    seq_t n1 = n0 + 1;
    seq_t t1 = n0 * n1;                /* n(n+1) */
    seq_t poly = 3 * n0 * n0 + 7 * n0 + 8; /* 3n^2 + 7n + 8 */
    seq_t prod = t1 * poly;
    return prod / 12;
}
