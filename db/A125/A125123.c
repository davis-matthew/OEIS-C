/*
 * A125123  Decimal expansion of the semi-major axis (in meters) of the World
 *          Geodetic System 1984 Ellipsoid, second upgrade.
 * https://oeis.org/A125123
 *
 * Offset:    7    (a(7) is the first term)
 * Keywords:  cons, nonn
 * Formula:   a(n) = 0 for n<7; for 7 ≤ n ≤ 13, a(n) = the (n-6)-th digit of
 *            6378137, otherwise a(n)=0
 * Range:     u8 n <= 13, u16 n <= 13, u32 n <= 13, u64 n <= 13, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* digits of 6378137 from most significant to least */
    static const unsigned char digits[7] = {6,3,7,8,1,3,7};
    if (n < 7)
        return 0;
    idx_t i = n - 7;
    if (i < 7)
        return (seq_t)digits[i];
    return 0;
}
