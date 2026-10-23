/*
 * A029928  Convert n from statute miles to nautical miles.
 * https://oeis.org/A029928
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = round( n / 1.15078…) = floor( (n*1609 + 926)/1852 )
 * Range:     u8 none, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    return ((seq_t)n * (seq_t)1609 + (seq_t)926) / (seq_t)1852;
}
