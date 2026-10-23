/*
 * A033353  a(n) = floor(33/n).
 * https://oeis.org/A033353
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = floor(33/n)
 * Range:     u8 n <= 91, u16 n <= 91, u32 n <= 91, u64 n <= 91, u128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Integer division using seq_t for the numerator guarantees
       correct results even when seq_t is smaller than idx_t. */
    return ((seq_t)33) / n;
}
