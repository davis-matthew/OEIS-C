/*
 * A167508  Number of different letters in the French spelling of the number
 *          n.
 * https://oeis.org/A167508
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, word, changed
 * Formula:   not stated
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Validated: partial, pass, 91 terms through n = 90 (2026-11-03)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the number of distinct letters in the French spelling of n
   (letters a–z only, accents and punctuation ignored).  The function
   follows the standard French numbering rules up to arbitrary size. */

#include <stdint.h>
#include <stddef.h>

/* Add all letters of an ASCII word to the bit mask.
   Only characters 'a'..'z' are considered. */
static void add_word_mask(const char *w, uint32_t *mask)
{
    for (; *w; ++w) {
        char c = *w;
        if (c >= 'a' && c <= 'z')
            *mask |= 1u << (c - 'a');
    }
}

/* Recursive routine that builds the mask for a number < 1000. */
static void add_sub(uint32_t sub, uint32_t *mask)
{
    /* Numbers 1 … 16 */
    static const char *under20[20] = {
        "",        "un",   "deux",  "trois", "quatre", "cinq",
        "six",     "sept", "huit", "neuf",  "dix",    "onze",
        "douze",   "treize", "quatorze", "quinze", "seize",
        "dix-sept", "dix-huit", "dix-neuf"
    };

    if (sub == 0)
        return;

    if (sub < 20) {
        add_word_mask(under20[sub], mask);
        return;
    }

    if (sub < 100) {
        unsigned tens = sub / 10;
        unsigned rem  = sub % 10;

        if (tens == 7) {                      /* 70‑79: soixante + 10‑19 */
            if (rem == 11) {                  /* 71: soixante et onze */
                add_word_mask("soixante", mask);
                add_word_mask("et", mask);
                add_word_mask("onze", mask);
            } else {
                add_word_mask("soixante", mask);
                add_sub(10 + rem, mask);      /* 10‑19 */
            }
        } else if (tens == 8) {               /* 80‑89 */
            if (rem == 0) {
                add_word_mask("quatre-vingts", mask);
            } else {
                add_word_mask("quatre-vingt", mask);
                add_sub(rem, mask);
            }
        } else if (tens == 9) {               /* 90‑99 */
            add_word_mask("quatre-vingt", mask);
            add_sub(sub - 80, mask);          /* 10‑19 */
        } else {                              /* 20‑69 */
            static const char *tens_word[7] = {
                "", "", "vingt", "trente", "quarante",
                "cinquante", "soixante"
            };
            if (rem == 0) {
                add_word_mask(tens_word[tens], mask);
            } else if (rem == 1) {          /* 21,31,...,61 */
                add_word_mask(tens_word[tens], mask);
                add_word_mask("et", mask);
                add_word_mask("un", mask);
            } else {
                add_word_mask(tens_word[tens], mask);
                add_sub(rem, mask);
            }
        }
        return;
    }

    /* 100‑999 */
    unsigned hundreds = sub / 100;
    unsigned rem      = sub % 100;

    if (hundreds == 1)
        add_word_mask("cent", mask);
    else {
        add_sub(hundreds, mask);          /* 1‑9 */
        add_word_mask("cent", mask);
    }

    if (rem == 0) {
        if (hundreds > 1)
            *mask |= 1u << ('s' - 'a');    /* plural "cents" */
    } else {
        add_sub(rem, mask);
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    uint64_t val = (uint64_t)n;
    uint32_t mask = 0;

    if (val == 0) {
        add_word_mask("zero", &mask);
    } else {
        /* Split into groups of 3 digits (units, thousands, millions, …) */
        uint32_t groups[10];
        int gcnt = 0;
        while (val > 0 && gcnt < 10) {
            groups[gcnt++] = (uint32_t)(val % 1000);
            val /= 1000;
        }

        /* Names for powers of 1000 starting with million (index 2) */
        static const char *power_name[6] = {
            "million", "milliard", "billion",
            "billiard", "trillion", "trilliard"
        };

        for (int i = gcnt - 1; i >= 0; --i) {
            uint32_t g = groups[i];
            if (g == 0)
                continue;

            if (i == 0) {                       /* units */
                add_sub(g, &mask);
            } else if (i == 1) {                /* thousands */
                if (g == 1)
                    add_word_mask("mille", &mask);
                else {
                    add_sub(g, &mask);
                    add_word_mask("mille", &mask);
                }
            } else {                            /* millions and higher */
                add_sub(g, &mask);
                if (i - 2 < (int)(sizeof(power_name)/sizeof(power_name[0]))) {
                    add_word_mask(power_name[i - 2], &mask);
                    if (g > 1)
                        mask |= 1u << ('s' - 'a');
                }
            }
        }
    }

    /* Count set bits in mask = number of distinct letters */
    int cnt = 0;
    uint32_t m = mask;
    while (m) {
        m &= m - 1;
        ++cnt;
    }
    return (seq_t)cnt;
}
