/*
 * A008217  a(n) = floor(n/4)*floor((n+1)/4).
 * https://oeis.org/A008217
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n/4) * floor((n+1)/4)
 * Range:     u8 n <= 63, u16 n <= 1023, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t q = ((seq_t)n) / ((seq_t)4);
    seq_t r = (((seq_t)n) + ((seq_t)1)) / ((seq_t)4);
    return q * r;
}
