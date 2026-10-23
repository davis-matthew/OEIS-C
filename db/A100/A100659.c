/*
 * A100659  Floor of measure (in degrees) of the internal angles of a regular
 *          polygon with n sides.
 * https://oeis.org/A100659
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 179 - floor(359 / n)
 * Range:     u8 n <= 56, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* floor(359 / n) is computed with the native 64‑bit type idx_t.
       The result fits into seq_t because it is at most 359. */
    idx_t t = 359 / n;
    return (seq_t)(179 - (seq_t)t);
}
