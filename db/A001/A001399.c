/*
 * A001399  a(n) is the number of partitions of n into at most 3 parts; also
 *          partitions of n+3 in which the greatest part is 3; also number of
 *          unlabeled multigraphs with 3 nodes and n edges.
 * https://oeis.org/A001399
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = round((n + 3)^2 / 12) = ((n + 3)^2 + 6) / 12
 * Range:     u8 n <= 12, u16 n <= 882, u32 n <= 9548, u64 n <= 9548,
 *            u128 n <= 9548, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t n_plus_3 = (seq_t)n + 3;
    seq_t numerator = n_plus_3 * n_plus_3 + 6;
    return numerator / 12;
}
