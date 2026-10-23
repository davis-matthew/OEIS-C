/*
 * A029918  Convert n from meters to feet.
 * https://oeis.org/A029918
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(n * 3.28084 + 0.5) which is equivalent to rounding
 *            the product n*328084/100000 to the nearest integer
 * Range:     u8 n <= 59, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute (n*328084 + 50000) / 100000 using seq_t arithmetic. */
    return ((seq_t)n * 328084 + 50000) / 100000;
}
