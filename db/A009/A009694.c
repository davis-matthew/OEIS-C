/*
 * A009694  a(n) = Product_{i=0..7} floor((n+i)/8).
 * https://oeis.org/A009694
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = product_{i=0..7} floor((n+i)/8)
 * Range:     u8 n <= 15, u16 n <= 31, u32 n <= 127, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 1;
    for (idx_t i = 0; i < 8; ++i)
    {
        seq_t factor = ((seq_t)(n + i)) / 8;
        result *= factor;
    }
    return result;
}
