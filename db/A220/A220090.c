/*
 * A220090  Braille natural numbers (including zero), using "0" as digit
 *          concatenation mark.
 * https://oeis.org/A220090
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn, changed
 * Formula:   a(n) = concatenate the Braille patterns of the decimal digits of
 *            n with "0" as a separator. The mapping is 0 → 245, 1 → 1, 2 →
 *            12, 3 → 14, 4 → 145, 5 → 15, 6 → 124, 7 → 1245, 8 → 125, 9 → 24
 * Range:     u8 n <= 6, u16 n <= 16, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

/* Braille mapping for decimal digits (0‑9) */
static const char * const braille_map[10] = {
    "245","1","12","14","145","15","124","1245","125","24"
};

seq_t a(idx_t n)
{
    /* Extract decimal digits of n (least significant first) */
    unsigned digits[21];
    int len = 0;

    if (n == 0) {
        digits[0] = 0;
        len = 1;
    } else {
        idx_t tmp = n;
        while (tmp > 0) {
            digits[len++] = (unsigned)(tmp % 10);
            tmp /= 10;
        }
    }

    seq_t result = 0;

    /* Process digits from most significant to least */
    for (int i = len - 1; i >= 0; --i) {
        const char *pat = braille_map[digits[i]];
        for (const char *p = pat; *p; ++p) {
            result = result * 10 + (seq_t)(*p - '0');
        }
        if (i > 0) {          /* separator '0' between patterns */
            result = result * 10;
        }
    }

    return result;
}
