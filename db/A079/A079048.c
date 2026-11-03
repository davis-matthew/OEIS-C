/*
 * A079048  Numerical equivalents of the words zero, one, two, three, ... on
 *          touch-tone telephone.
 * https://oeis.org/A079048
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, word, base
 * Formula:   a(n) = telephone keypad digits of n written in English words
 *            (e.g., 0 → "zero" → 9376, 21 → "twenty one" → 893689663)
 * Range:     u8 none, u16 n <= 2, u32 n <= 22, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Validated: partial, pass, 27 terms through n = 26 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <ctype.h>
#include <string.h>

/* Append a word to the buffer, adding a leading space if needed. */
static void add_word(char *buf, int *pos, const char *w, int *first) {
    if (!*first) buf[(*pos)++] = ' ';
    while (*w) buf[(*pos)++] = *w++;
    *first = 0;
}

/* Map a letter to its telephone keypad digit. */
static unsigned digit_from_char(char c) {
    c = toupper((unsigned char)c);
    switch (c) {
        case 'A': case 'B': case 'C': return 2;
        case 'D': case 'E': case 'F': return 3;
        case 'G': case 'H': case 'I': return 4;
        case 'J': case 'K': case 'L': return 5;
        case 'M': case 'N': case 'O': return 6;
        case 'P': case 'Q': case 'R': case 'S': return 7;
        case 'T': case 'U': case 'V': return 8;
        case 'W': case 'X': case 'Y': case 'Z': return 9;
        default: return 0;   /* non‑letters map to 0 (e.g., space) */
    }
}

static const char *ones[]  = {"zero","one","two","three","four","five",
                              "six","seven","eight","nine"};
static const char *teens[] = {"ten","eleven","twelve","thirteen","fourteen",
                              "fifteen","sixteen","seventeen","eighteen","nineteen"};
static const char *tens[]  = {"","", "twenty","thirty","forty",
                               "fifty","sixty","seventy","eighty","ninety"};

/* Main sequence function. */
seq_t a(idx_t n) {
    /* Buffer large enough for words up to a few billions. */
    char word[256];
    int pos   = 0;
    int first = 1;   /* whether we are adding the first word */

    if (n == 0) {
        add_word(word, &pos, "zero", &first);
    } else {
        idx_t rem = n;

        /* Handle thousands, hundreds, tens, units. */
        if (rem >= 1000) {
            idx_t thousands = rem / 1000;
            rem %= 1000;
            if (thousands == 1) {
                /* For 1000–1999, write "one thousand". */
                add_word(word, &pos, "one", &first);
            } else {
                /* For 2000 and above. */
                add_word(word, &pos, ones[thousands], &first);
            }
            add_word(word, &pos, "thousand", &first);
        }

        if (rem >= 100) {
            idx_t hundreds = rem / 100;
            rem %= 100;
            add_word(word, &pos, ones[hundreds], &first);
            add_word(word, &pos, "hundred", &first);
        }

        if (rem >= 20) {
            idx_t t = rem / 10;
            rem %= 10;
            add_word(word, &pos, tens[t], &first);
            if (rem > 0) {
                add_word(word, &pos, ones[rem], &first);
            }
        } else if (rem >= 10) {
            add_word(word, &pos, teens[rem-10], &first);
        } else if (rem > 0) {
            add_word(word, &pos, ones[rem], &first);
        }
    }

    /* Convert the word to the numeric keypad value. */
    seq_t result = 0;
    for (int i = 0; i < pos; ++i) {
        char c = word[i];
        if (c == ' ') continue;  /* skip spaces */
        unsigned d = digit_from_char(c);   /* 0–9 */
        result = result * 10 + (seq_t)d;
    }
    return result;
}
