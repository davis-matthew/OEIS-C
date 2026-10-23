/*
 * A037195  Number of consonants in the American English name of n.
 * https://oeis.org/A037195
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, word, easy
 * Formula:   a(n) = number of consonants in the American English name of n
 * Range:     u8 n <= 103, u16 n <= 103, u32 n <= 103, u64 n <= 103,
 *            u128 n <= 103, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* Helper: count consonants in a single word (no spaces or hyphens). */
static void add_word(const char *word, seq_t *cnt)
{
    for (const char *p = word; *p; ++p) {
        unsigned char c = (unsigned char)*p;
        if (c >= 'A' && c <= 'Z')
            c = c - 'A' + 'a';
        if (c >= 'a' && c <= 'z') {
            if (c != 'a' && c != 'e' && c != 'i' && c != 'o' &&
                c != 'u' && c != 'y')
                (*cnt)++;
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 2; /* "zero" -> z, r */

    /* Number names for 0-19 */
    static const char *units[] = {
        "", "one", "two", "three", "four",
        "five", "six", "seven", "eight", "nine",
        "ten", "eleven", "twelve", "thirteen",
        "fourteen", "fifteen", "sixteen", "seventeen",
        "eighteen", "nineteen"
    };

    /* Tens 20-90 */
    static const char *tens[] = {
        "", "", "twenty", "thirty", "forty",
        "fifty", "sixty", "seventy", "eighty", "ninety"
    };

    /* Scale names for groups of 3 digits */
    static const char *scale[] = {
        "", "thousand", "million", "billion", "trillion",
        "quadrillion", "quintillion", "sextillion",
        "septillion", "octillion", "nonillion"
    };

    seq_t count = 0;
    idx_t temp = n;
    size_t group = 0; /* 0 = units, 1 = thousand, etc. */

    while (temp > 0) {
        int num = (int)(temp % 1000);
        if (num != 0) {
            /* Hundreds */
            int h = num / 100;
            int rem = num % 100;
            if (h > 0) {
                add_word(units[h], &count);
                add_word("hundred", &count);
            }
            /* Tens and units */
            if (rem >= 20) {
                int t = rem / 10;
                int u = rem % 10;
                add_word(tens[t], &count);
                if (u > 0)
                    add_word(units[u], &count);
            } else if (rem > 0) {
                add_word(units[rem], &count);
            }
            /* Scale suffix */
            if (group > 0)
                add_word(scale[group], &count);
        }
        temp /= 1000;
        ++group;
    }

    return count;
}
