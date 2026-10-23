/*
 * A125125  Decimal expansion of the geocentric gravitational constant (mass
 *          of Earth's atmosphere included) of the World Geodetic System 1984
 *          Ellipsoid, second upgrade.
 * https://oeis.org/A125125
 *
 * Offset:    15    (a(15) is the first term)
 * Keywords:  cons, nonn
 * Formula:   a(n) = the (n‑15)‑th digit of the decimal integer
 *            3986004418000000, padding with zeros for n beyond the integer
 *            length
 * Range:     u8 n <= 24, u16 n <= 24, u32 n <= 24, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* decimal digits of 3986004418000000 */
    static const char digits[] = "3986004418000000";

    if (n < 15)
        return 0;                    /* off‑offset behaviour arbitrary */

    idx_t pos = n - 15;            /* 0‑based position in the string */

    if (pos < (sizeof(digits) - 1))
        return (seq_t)(digits[pos] - '0');

    /* past the last known digit → return 0 (trailing zeros) */
    return 0;
}
