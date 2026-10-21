/*
 * A070062  Decimal expansion of old estimate for Avogadro's constant (in
 *          mol^(-1)).
 * https://oeis.org/A070062
 *
 * Offset:    24    (a(24) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = the (n-24+1)-th decimal digit of
 *            602214199000000000000000; digits beyond the 24 digits are zero
 * Range:     i8 n <= 32, i16 n <= 32, i32 n <= 32, i64 n <= 32, i128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 24) return 0;
    idx_t i = (idx_t)(n - 24);           /* offset 24 → digit index 0…23   */
    static const uint8_t digits[24] = {
        6,0,2,2,1,4,1,9,9,          /* 602214199            */
        0,0,0,0,0,0,0,0,0,0,0,0,0,0 /* 15 trailing zeros   */
    };
    if (i < 24) return (seq_t)digits[i];
    return 0;
}
