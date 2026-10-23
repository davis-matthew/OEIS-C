/*
 * A029925  Convert n from degrees Celsius to nearest integer Fahrenheit.
 * https://oeis.org/A029925
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((18*n + 325)/10)
 * Range:     u8 none, u16 n <= 2000, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t temp = 18 * (seq_t)n + 325;
    return temp / 10;
}
