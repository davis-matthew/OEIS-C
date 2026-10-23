/*
 * A126582  Delete all letters except i, v, x, l, c, d and m from n (in
 *          French) and read as a Roman numeral if possible, otherwise 0.
 * https://oeis.org/A126582
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, word
 * Formula:   a(n) = sum over Roman letters extracted from the French word for
 *            n, interpreted as a Roman numeral if valid, otherwise 0
 * Range:     u8 n <= 1, u16 n <= 20, u32 n <= 20, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   append_roman_from_word, canonical_roman
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <ctype.h>
#include "../utils/append_roman_from_word.h"
#include "../utils/canonical_roman.h"

/* Roman letter values */
static int roman_value(char c)
{
    switch (c) {
        case 'I': return 1;
        case 'V': return 5;
        case 'X': return 10;
        case 'L': return 50;
        case 'C': return 100;
        case 'D': return 500;
        case 'M': return 1000;
        default:  return 0;
    }
}

/* Number words 0…19 (French). */
static const char *const small[20] = {
    "ZERO","UN","DEUX","TROIS","QUATRE","CINQ","SIX","SEPT","HUIT","NEUF",
    "DIX","ONZE","DOUZE","TREIZE","QUATORZE","QUINZE","SEIZE",
    "DIX-SEPT","DIX-HUIT","DIX-NEUF"
};

/* Build Roman letters for numbers < 20. */
static void roman_from_below20(idx_t n, char *buf, size_t *len)
{
    append_roman_from_word(small[n], buf, len);
}

/* Build Roman letters for numbers < 100. */
static void roman_from_below100(idx_t n, char *buf, size_t *len)
{
    if (n < 20) {
        roman_from_below20(n, buf, len);
        return;
    }

    if (n < 70) {
        int tens = n / 10;
        int unit = n % 10;
        static const char *tens_word[5] = {
            "VINGT","TRENTE","QUARANTE","CINQUANTE","SOIXANTE"
        };
        append_roman_from_word(tens_word[tens - 2], buf, len);
        if (unit > 0)
            append_roman_from_word(small[unit], buf, len);
    }
    else if (n < 80) {
        int rem = n - 60;              /* 10 … 19 */
        append_roman_from_word("SOIXANTE", buf, len);
        append_roman_from_word(small[rem], buf, len);
    }
    else { /* 80 … 99 */
        int rem = n - 80;              /* 10 … 19 */
        append_roman_from_word("QUATREVINGT", buf, len);
        if (rem > 0)
            append_roman_from_word(small[rem], buf, len);
    }
}

/* Build Roman letters for numbers < 1000. */
static void roman_from_below1000(idx_t n, char *buf, size_t *len)
{
    if (n < 100) {
        roman_from_below100(n, buf, len);
        return;
    }

    int hundreds = n / 100;
    int rem = n % 100;

    if (hundreds == 1) {
        append_roman_from_word("CENT", buf, len);
    } else {
        roman_from_below20(hundreds, buf, len);  /* e.g., DEUX, TROIS, ... */
        append_roman_from_word("CENT", buf, len);
    }

    if (rem > 0)
        roman_from_below100(rem, buf, len);
}

/* Build Roman letters for any n < 10000. */
static void roman_from_number(idx_t n, char *buf, size_t *len)
{
    if (n == 0)
        return;        /* ZERO gives no Roman letters */

    idx_t thousands = n / 1000;
    idx_t rem = n % 1000;

    for (idx_t i = 0; i < thousands; ++i)
        buf[(*len)++] = 'M';

    if (rem > 0)
        roman_from_below1000(rem, buf, len);
}

/* Parse and validate a Roman numeral string. */
static seq_t parse_roman(const char *s, size_t len)
{
    if (len == 0)
        return 0;

    seq_t total = 0;
    size_t i = 0;

    while (i < len) {
        int val = roman_value(s[i]);
        if (val == 0)
            return 0;

        int next = 0;
        if (i + 1 < len)
            next = roman_value(s[i + 1]);

        if (next > val) {
            /* Subtractive pair – only allowed combinations. */
            if (!((val == 1 && (next == 5 || next == 10)) ||
                  (val == 10 && (next == 50 || next == 100)) ||
                  (val == 100 && (next == 500 || next == 1000))))
                return 0;
            total += (seq_t)(next - val);
            i += 2;
        } else {
            total += val;
            i += 1;
        }
    }

    /* Verify canonical form. */
    char canon[64];
    size_t canon_len;
    canonical_roman(total, canon, &canon_len);

    if (canon_len != len)
        return 0;
    for (size_t k = 0; k < len; ++k)
        if (canon[k] != s[k])
            return 0;

    return total;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    char buf[64];
    size_t len = 0;
    roman_from_number(n, buf, &len);
    return parse_roman(buf, len);
}
