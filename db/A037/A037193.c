/*
 * A037193  Number of consonants in n (in French).
 * https://oeis.org/A037193
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, word, easy
 * Formula:   a(n) = number of consonants in the French word for n. * The
 *            function works for all n ≥ 0 (up to 2^64 − 1). * It decomposes n
 *            into groups of three digits and adds * the consonant counts of
 *            the small numbers (0‑999) together * with the appropriate
 *            magnitude word ("mille", "million", …). * The imple...
 * Range:     u8 n <= 103, u16 n <= 103, u32 n <= 103, u64 n <= 103,
 *            u128 n <= 103, big any n
 * Helpers:   _magnitude_plural, _magnitude_singular
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>   /* for uint64_t; idx_t is defined elsewhere */
#include "../utils/_magnitude_plural.h"
#include "../utils/_magnitude_singular.h"

/* Consonant counts for the units 0–19. */
static const seq_t _UNITS[20] = {
    2, /* zéro     */
    1, /* un       */
    2, /* deux     */
    3, /* trois    */
    3, /* quatre   */
    3, /* cinq     */
    2, /* six      */
    3, /* sept     */
    2, /* huit     */
    2, /* neuf     */
    2, /* dix      */
    2, /* onze     */
    2, /* douze    */
    3, /* treize   */
    4, /* quatorze */
    3, /* quinze   */
    2, /* seize    */
    5, /* dix-sept */
    4, /* dix-huit */
    4  /* dix-neuf */
};

/* Consonant counts for the tens 20–60 (increasing by 10). */
static const seq_t _TWENTY[5] = {4, 4, 4, 5, 4};      /* vingt, trente,
                                                     * quarante, cinquante,
                                                     * soixante */

/* --------------------------------------------------------------------
 * Return the consonant count for a number smaller than 1000.
 * -------------------------------------------------------------------- */
static seq_t _count_small(idx_t n)
{
    if (n == 0)                             /* not used for group 0 */
        return _UNITS[0];

    if (n < 20)
        return _UNITS[n];

    if (n < 100) {
        idx_t tens = n / 10;
        idx_t rem  = n % 10;

        /* 20–60, special treatment for the "et un" pattern. */
        if (tens >= 2 && tens <= 6) {
            if (rem == 0)
                return _TWENTY[tens - 2];
            if (rem == 1)                  /* vingt/… et un */
                return _TWENTY[tens - 2] + 2;
            return _TWENTY[tens - 2] + _UNITS[rem];
        }

        /* 70–79: soixante‑10/…/19, with 71 special. */
        if (tens == 7) {
            if (rem == 0)                  /* soixante‑dix */
                return 4 + 2;              /* 4 (soixante) + 2 (dix) */
            if (rem == 1)                  /* soixante‑et‑onze */
                return 4 + 1 + 2;          /* +et + onze */
            return 4 + _UNITS[rem + 10];   /* 4 + unit[10+rem] */
        }

        /* 80–89: quatre‑vingt‑X (no s), 80 itself has an s. */
        if (tens == 8) {
            if (rem == 0)                  /* quatre‑vingts */
                return 3 + 5;              /* 3 (quatre) + 5 (vingts) */
            return 3 + 4 + _UNITS[rem];    /* 3 + 4 (vingt) + unit */
        }

        /* 90–99: quatre‑vingt‑X (X 10–19). */
        if (tens == 9) {
            return 3 + 4 + _UNITS[rem + 10]; /* 3+4 (vingt) + unit[10+rem] */
        }
    }

    /* 100–999. */
    idx_t h    = n / 100;
    idx_t rest = n % 100;

    if (rest == 0) {
        if (h == 1)                    /* cent */
            return 3;                  /* c, n, t */
        return _UNITS[h] + 4;          /* e.g. deux cent(s) */
    }

    if (h == 1)
        return 3 + _count_small(rest); /* cent + rest */

    return _UNITS[h] + 3 + _count_small(rest); /* H cent + rest */
}

/* --------------------------------------------------------------------
 * Main sequence function.
 * -------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)
        return _UNITS[0];   /* zéro */

    seq_t total = 0;
    idx_t group = 0;        /* 0 = units, 1 = thousands, 2 = millions, … */
    idx_t x = n;

    while (x > 0) {
        idx_t grp_val = x % 1000;   /* 0–999 */
        if (grp_val != 0) {
            /* consonants of the small number itself */
            total += _count_small(grp_val);

            /* add the magnitude word if needed */
            if (group > 0) {
                if (grp_val == 1)
                    total += _magnitude_singular(group);
                else
                    total += _magnitude_plural(group);
            }
        }
        x /= 1000;
        group++;
    }
    return total;
}
