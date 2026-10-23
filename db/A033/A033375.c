/*
 * A033375  a(n) = floor(55/n).
 * https://oeis.org/A033375
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = floor(55/n)
 * Range:     u8 n <= 92, u16 n <= 92, u32 n <= 92, u64 n <= 92, u128 n <= 92,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For n >= 1, integer division gives floor(55/n). */
    /* No floating point is used; division is performed on an unsigned type. */
    return (seq_t)55 / n;
}
