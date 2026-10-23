/*
 * A152777  7 times heptagonal numbers: a(n) = 7*n*(5*n-3)/2.
 * https://oeis.org/A152777
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 7*n*(5*n-3)/2
 * Range:     u8 n <= 4, u16 n <= 61, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Work in seq_t to avoid overflows on intermediate results. */
    seq_t nn = (seq_t)n;
    seq_t term = 7 * nn * (5 * nn - 3) / 2;
    return term;
}
