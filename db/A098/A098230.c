/*
 * A098230  75-gonal numbers: a(n) = n*(73*n-71)/2.
 * https://oeis.org/A098230
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(73*n-71)/2
 * Range:     u8 n <= 2, u16 n <= 30, u32 n <= 7754, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t n_seq = (seq_t)n;
    seq_t prod = n_seq * ((n_seq * (seq_t)73) - (seq_t)71);
    return prod / (seq_t)2;
}
