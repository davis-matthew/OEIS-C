/*
 * A073427  Roman numerals for n evaluated as if in Sallows' base 27.
 * https://oeis.org/A073427
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = value of the Roman numeral of n interpreted as a base‑27
 *            number with A=1, B=2, …, Z=26
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    /* Roman numeral value/symbol pairs (descending order) */
    static const struct {
        idx_t val;
        const char *sym;
    } roman[] = {
        {1000, "M"}, {900, "CM"}, {500, "D"}, {400, "CD"},
        {100,  "C"}, {90,  "XC"}, {50,  "L"}, {40,  "XL"},
        {10,   "X"}, {9,   "IX"}, {5,   "V"}, {4,   "IV"},
        {1,    "I"}
    };
    const size_t roman_cnt = sizeof(roman) / sizeof(roman[0]);

    idx_t remaining = n;
    seq_t result = 0;

    for (size_t i = 0; i < roman_cnt; ++i) {
        while (remaining >= roman[i].val) {
            remaining -= roman[i].val;
            const char *s = roman[i].sym;
            while (*s) {
                int digit = (*s) - 'A' + 1;   /* A=1, B=2, …, Z=26 */
                result = result * 27 + digit;
                ++s;
            }
        }
    }
    return result;
}
