/*
 * A029920  Convert n from centimeters (cm) to inches (").
 * https://oeis.org/A029920
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = round(n / 2.54) = (n*100 + 127) / 254
 * Range:     u8 n <= 1, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert n to seq_t to avoid overflow and compute the rounded conversion */
    seq_t t = (seq_t)n * 100 + 127;
    return t / 254;
}
