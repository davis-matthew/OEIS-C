/*
 * A121019  Value of n written in Braille, seen as 2 X 3 bit-code: 1 for a
 *          raised dot and 0 for an empty dot position.
 * https://oeis.org/A121019
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = a(floor(n/10))*64 + a(n mod 10) with digit-to-Braille
 *            mapping: 0→22, 1→32, 2→48, 3→36, 4→38, 5→34, 6→52, 7→54, 8→50,
 *            9→20
 * Range:     u8 n <= 9, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Braille encodings for decimal digits 0-9 */
    static const seq_t braille[10] = {22,32,48,36,38,34,52,54,50,20};

    /* Special case: n == 0 */
    if (n == 0)
        return braille[0];

    /* Extract decimal digits (least significant first) */
    unsigned digits[20];
    unsigned cnt = 0;
    idx_t tmp = n;
    while (tmp > 0) {
        digits[cnt++] = tmp % 10;
        tmp /= 10;
    }

    /* Build the value from most significant digit to least */
    seq_t result = 0;
    for (int i = cnt - 1; i >= 0; --i)
        result = result * (seq_t)64 + braille[digits[i]];

    return result;
}
