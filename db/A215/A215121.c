/*
 * A215121  Strobogrammatic palindromic numbers in their Roman numeral
 *          representation.
 * https://oeis.org/A215121
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   a(n) = smallest number whose Roman numeral * representation is a
 *            strobogrammatic palindrome * (only the symbols I and X may
 *            appear and the string must read the * same left-to-right and
 *            right-to-left). * The function enumerates natural numbers until
 *            it finds the n‑th * such number. If n exceeds th...
 * Range:     u8 n <= 7, u16 n <= 7, u32 n <= 7, u64 n <= 7, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <string.h>
#include <stdbool.h>

#define MAX_SEARCH 200000     /* upper bound for the exhaustive search */

static const int roman_values[13] = {
    1000, 900, 500, 400,
     100,  90,  50,  40,
      10,   9,   5,   4,   1
};
static const char *roman_syms[13] = {
    "M", "CM", "D", "CD",
    "C", "XC", "L", "XL",
    "X", "IX", "V", "IV", "I"
};

/* Convert an integer to its standard Roman numeral string.
   The result is stored in buf (which must be large enough). */
static void to_roman(idx_t num, char *buf)
{
    int pos = 0;
    for (int i = 0; i < 13; ++i) {
        while (num >= roman_values[i]) {
            num -= roman_values[i];
            const char *sym = roman_syms[i];
            for (; *sym; ++sym)
                buf[pos++] = *sym;
        }
    }
    buf[pos] = '\0';
}

/* Test whether the Roman numeral representation of `num`
   uses only the symbols I and X and is a palindrome. */
static bool is_strob_pal(idx_t num)
{
    char buf[64];
    to_roman(num, buf);
    size_t len = strlen(buf);

    /* only I and X may appear */
    for (size_t i = 0; i < len; ++i) {
        char c = buf[i];
        if (c != 'I' && c != 'X')
            return false;
    }

    /* palindrome check */
    for (size_t a = 0, b = len - 1; a < b; ++a, --b) {
        if (buf[a] != buf[b])
            return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    idx_t count = 0;
    for (idx_t num = 1; num <= MAX_SEARCH; ++num) {
        if (is_strob_pal(num)) {
            ++count;
            if (count == n)
                return (seq_t)num;
        }
    }
    /* If the n‑th element has not been found within the search limit,
       return 0 (undefined for this sequence). */
    return 0;
}
