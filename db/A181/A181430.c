/*
 * A181430  Number of distinct 5-card poker hands using n ranks and 4
 *          unlabeled suits.
 * https://oeis.org/A181430
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n * (51 * n^4 - 110 * n^3 + 165 * n^2 - 130 * n + 24)/120
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 42, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t n2 = nn * nn;
    seq_t n3 = n2 * nn;
    seq_t n4 = n3 * nn;
    seq_t poly = (seq_t)51 * n4 - (seq_t)110 * n3 + (seq_t)165 * n2 - (seq_t)130 * nn + (seq_t)24;
    seq_t numerator = nn * poly;
    return numerator / 120;
}
