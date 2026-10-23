/*
 * A152995  Twice 11-gonal numbers: a(n) = n*(9*n-7).
 * https://oeis.org/A152995
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n*(9*n-7)
 * Range:     u8 n <= 5, u16 n <= 85, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t part = (seq_t)9 * nn - (seq_t)7;
    return nn * part;
}
