/*
 * A062726  Numbers that do not contain repeated letters when written in Roman
 *          numerals.
 * https://oeis.org/A062726
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 58, u16 n <= 316, u32 n <= 316, u64 n <= 316,
 *            u128 n <= 316, big any n
 * Validated: full, pass, 316 terms through n = 316 (2026-11-02)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for the nth integer whose Roman numeral representation contains no repeated letters. */
#include <stdbool.h>

static bool has_unique_roman_digits(int x)
{
    /* Roman letters: I V X L C D M – 7 distinct symbols */
    bool seen[7] = {false};

    static const int values[13] =
        {1000, 900, 500, 400, 100, 90, 50, 40, 10, 9, 5, 4, 1};
    static const char *symbols[13] =
        {"M", "CM", "D", "CD", "C", "XC", "L", "XL",
         "X", "IX", "V", "IV", "I"};

    for (int i = 0; i < 13; ++i) {
        while (x >= values[i]) {
            const char *p = symbols[i];
            while (*p) {
                int idx;
                switch (*p) {
                case 'I': idx = 0; break;
                case 'V': idx = 1; break;
                case 'X': idx = 2; break;
                case 'L': idx = 3; break;
                case 'C': idx = 4; break;
                case 'D': idx = 5; break;
                case 'M': idx = 6; break;
                default: return false; /* unexpected symbol */
                }
                if (seen[idx]) return false;   /* repeated letter */
                seen[idx] = true;
                ++p;
            }
            x -= values[i];
        }
    }
    return true;
}

seq_t a(idx_t n)
{
    /* The OEIS sequence has finitely many terms (maximum is 1666). */
    if (n == 0) return (seq_t)0;  /* undefined for n < 1 */

    int count = 0;
    int candidate = 1;
    while (candidate <= 1666) {
        if (has_unique_roman_digits(candidate)) {
            ++count;
            if ((idx_t)count == n) {
                return (seq_t)candidate;
            }
        }
        ++candidate;
    }

    /* n is too large – sequence ends at a(316) = 1666. */
    return (seq_t)0;
}
