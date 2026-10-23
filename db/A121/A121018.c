/*
 * A121018  Value of n written in Braille, seen as 3 X 2 bit-code: 1 for a
 *          raised dot and 0 for an empty dot position.
 * https://oeis.org/A121018
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = a(floor(n/10))*64 + a(n mod 10) (n > 9); Initial values:
 *            a(0) = 28, a(1) = 32, a(2) = 40, a(3) = 48, a(4) = 52, a(5) =
 *            36, a(6) = 56, a(7) = 60, a(8) = 44, a(9) = 24
 * Range:     u8 n <= 9, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Braille code values for digits 0-9 */
    static const seq_t digit_value[10] = {
        28, 32, 40, 48, 52, 36, 56, 60, 44, 24
    };

    /* For single-digit indices, return precomputed value */
    if (n < 10)
        return digit_value[(int)n];

    /* Extract decimal digits into a small array (max 20 for 64-bit). */
    idx_t digits[20];
    int count = 0;
    idx_t temp = n;

    while (temp > 0) {
        digits[count++] = temp % 10u;
        temp /= 10u;
    }

    /* Build the result using base-64 accumulation. */
    seq_t result = 0;
    for (int i = count - 1; i >= 0; --i) {
        /* Left shift by 6 bits = multiply by 64 */
        result = (result << 6) + digit_value[(int)digits[i]];
    }

    return result;
}
