/*
 * A160659  Numbers that have an "r" in the middle of their names.
 * https://oeis.org/A160659
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, word, less
 * Formula:   numbers whose English word representation has an 'r' at the
 *            middle character (counting letters only)
 * Range:     u8 n <= 13, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   copy_char
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/copy_char.h"

static const char *UNITS[20] = {
    "zero",   "one",     "two",   "three", "four",   "five",
    "six",    "seven",   "eight", "nine",  "ten",    "eleven",
    "twelve", "thirteen","fourteen","fifteen","sixteen",
    "seventeen","eighteen","nineteen"
};

static const char *TENS[10] = {
    "",      "",        "twenty", "thirty", "forty",
    "fifty", "sixty",   "seventy", "eighty", "ninety"
};

/* Recursive conversion of a non‑negative number (except 0) into its
   English word representation, concatenated without spaces or hyphens. */
static void num_words(uint64_t n, char **pos)
{
    /* Scale hierarchy: billion, million, thousand, hundred */
    if (n >= 1000000000ULL) {
        num_words(n / 1000000000ULL, pos);
        *pos = copy_char("billion", *pos);
        n %= 1000000000ULL;
    }
    if (n >= 1000000ULL) {
        num_words(n / 1000000ULL, pos);
        *pos = copy_char("million", *pos);
        n %= 1000000ULL;
    }
    if (n >= 1000ULL) {
        num_words(n / 1000ULL, pos);
        *pos = copy_char("thousand", *pos);
        n %= 1000ULL;
    }
    if (n >= 100ULL) {
        num_words(n / 100ULL, pos);
        *pos = copy_char("hundred", *pos);
        n %= 100ULL;
    }

    if (n >= 20ULL) {
        *pos = copy_char(TENS[n / 10], *pos);
        n %= 10ULL;
    }
    if (n > 0) {
        *pos = copy_char(UNITS[n], *pos);
    }
}

/* Return true iff the concatenated English spelling of n
   has an 'r' as the exact middle character. */
static bool has_middle_r(uint64_t n)
{
    char buf[256];
    char *ptr = buf;
    if (n == 0) {
        ptr = copy_char("zero", ptr);
    } else {
        num_words(n, &ptr);
    }
    size_t len = ptr - buf;
    if ((len & 1) == 0) // even length, no single middle
        return false;
    return buf[len / 2] == 'r';
}

/* Sequence a(n) with offset 1. */
extern seq_t a(idx_t n)
{
    if (n == 0) return 0; /* Not required but harmless */

    idx_t cnt = 0;
    idx_t num = 0;
    while (cnt < n) {
        ++num;
        if (has_middle_r(num)) {
            ++cnt;
        }
    }
    return (seq_t)num;
}
