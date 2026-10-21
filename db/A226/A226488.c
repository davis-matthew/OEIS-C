/*
 * A226488  a(n) = n*(13*n - 9)/2.
 * https://oeis.org/A226488
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(13*n - 9)/2
 * Range:     u8 n <= 4, u16 n <= 99, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    /* Compute n * (13*n - 9) first, then divide by 2. */
    seq_t prod = nn * ((nn * (seq_t)13 - (seq_t)9));
    return prod / 2;
}
