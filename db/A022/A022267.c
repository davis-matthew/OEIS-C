/*
 * A022267  a(n) = n*(9*n + 1)/2.
 * https://oeis.org/A022267
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(9*n + 1)/2
 * Range:     u8 n <= 5, u16 n <= 119, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t nn     = (seq_t)n;
    seq_t nine_n = nn * (seq_t)9;
    seq_t term   = nine_n + (seq_t)1;
    seq_t prod   = nn * term;
    return prod / (seq_t)2;
}
