/*
 * A164135  Numbers k such that k^2 == 2 (mod 47).
 * https://oeis.org/A164135
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 712, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = (47 + 19*(-1)^n + 94*(n-1))/4
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                 /* out of the defined range, safe guard */

    /* numerator = 47 + 94*(n-1) + 19*(-1)^n */
    seq_t num = 47 + 94 * (seq_t)(n - 1);

    /* add or subtract 19 depending on parity of n */
    if ((n & 1) == 0)
        num += 19;   /* even n => (-1)^n = 1 */
    else
        num -= 19;   /* odd n  => (-1)^n = -1 */

    return num / 4;
}
