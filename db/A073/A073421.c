/*
 * A073421  Roman numerals for n evaluated as if in base 36.
 * https://oeis.org/A073421
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = value of the Roman numeral of n interpreted as a base‑36
 *            number
 * Range:     u8 n <= 1, u16 n <= 7, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   roman_char_to_digit
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/roman_char_to_digit.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* Define Roman numeral values and symbols in descending order. */
    static const idx_t  VALUES[13] = {1000, 900, 500, 400,
                                      100,  90,  50,  40,
                                      10,    9,   5,   4,
                                      1};
    static const char *SYMBOLS[13] = {"M", "CM", "D", "CD",
                                      "C", "XC", "L", "XL",
                                      "X", "IX", "V", "IV",
                                      "I"};

    seq_t result = 0;
    idx_t i;

    for (i = 0; i < 13; ++i) {
        while (n >= VALUES[i]) {
            n -= VALUES[i];
            const char *s = SYMBOLS[i];
            for (; *s; ++s) {
                int digit = roman_char_to_digit(*s);
                result = result * 36 + digit;
            }
        }
    }
    return result;
}
