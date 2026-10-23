/*
 * A029722  Smallest positive integer containing the n-th letter of the
 *          alphabet (in US English), or -1 if no such integer exists.
 * https://oeis.org/A029722
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, sign, full, word
 * Formula:   a(n) is the smallest positive integer whose English name
 *            contains the n‑th letter of the US alphabet, or -1 if no such
 *            integer exists
 * Range:     i8 none, i16 none, i32 none, i64 n <= 2, i128 n <= 26, big any n
 * Helpers:   contains_letter, pow10seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <ctype.h>
#include <stdio.h>
#include "../utils/contains_letter.h"
#include "../utils/pow10seq.h"

seq_t a(idx_t n)
{
    /* Only the first 26 letters are defined. */
    if (n < 1 || n > 26)
        return (seq_t)-1;

    char target = 'A' + (n - 1);

    /* Words for numbers 1 to 19. */
    static const char *ones[20] = {
        "", "one", "two", "three", "four", "five", "six", "seven", "eight",
        "nine", "ten", "eleven", "twelve", "thirteen", "fourteen", "fifteen",
        "sixteen", "seventeen", "eighteen", "nineteen"
    };

    /* Words for tens (20,30,...,90). */
    static const char *tens[10] = {
        "", "", "twenty", "thirty", "forty", "fifty", "sixty",
        "seventy", "eighty", "ninety"
    };

    /* Scale names for powers of 10. */
    static const idx_t exps[9] = { 3, 6, 9, 12, 15, 18, 21, 24, 27 };
    static const char *scales[9] = {
        "thousand", "million", "billion", "trillion",
        "quadrillion", "quintillion", "sextillion",
        "septillion", "octillion"
    };

    seq_t best = 0;
    int found = 0;

    /* 1 to 19. */
    for (int i = 1; i <= 19; ++i) {
        if (contains_letter(ones[i], target)) {
            seq_t val = (seq_t)i;
            if (!found || val < best) {
                best = val;
                found = 1;
            }
        }
    }

    /* Tens 20,30,...,90. */
    for (int i = 2; i <= 9; ++i) {
        if (contains_letter(tens[i], target)) {
            seq_t val = (seq_t)(i * 10);
            if (!found || val < best) {
                best = val;
                found = 1;
            }
        }
    }

    /* 100 ("one hundred"). */
    if (contains_letter("one hundred", target)) {
        seq_t val = 100;
        if (!found || val < best) {
            best = val;
            found = 1;
        }
    }

    /* Powers of ten using scale names. */
    for (int k = 0; k < 9; ++k) {
        idx_t e = exps[k];
        /* Skip values that do not fit into fixed width types. */
        if (sizeof(seq_t) <= 8 && e > 18)
            continue;

        /* Build name "one <scale>". */
        char buf[64];
        snprintf(buf, sizeof buf, "one %s", scales[k]);

        if (contains_letter(buf, target)) {
            seq_t val = pow10seq(e);
            if (!found || val < best) {
                best = val;
                found = 1;
            }
        }
    }

    return found ? best : (seq_t)-1;
}
