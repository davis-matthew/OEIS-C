/*
 * A140675  a(n) = n*(3*n + 19)/2.
 * https://oeis.org/A140675
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(3*n + 19)/2
 * Range:     u8 n <= 6, u16 n <= 182, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t three_n_plus_19 = (seq_t)3 * nn + 19;
    seq_t prod = nn * three_n_plus_19;
    return prod / 2;
}
