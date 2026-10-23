/*
 * A167507  Number of letters in the French spelling of the number n, not
 *          counting hyphens and spaces.
 * https://oeis.org/A167507
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, word, changed
 * Formula:   a(n) = number of letters in the French spelling of n (hyphens
 *            and spaces omitted)
 * Range:     u8 n <= 81, u16 n <= 81, u32 n <= 81, u64 n <= 81, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Precomputed lengths of the words for 0‑19 */
static const int unit_len[20] = {
    4, /* zéro  */
    2, /* un    */
    4, /* deux  */
    5, /* trois */
    6, /* quatre */
    4, /* cinq  */
    3, /* six   */
    4, /* sept  */
    4, /* huit  */
    4, /* neuf  */
    3, /* dix   */
    4, /* onze  */
    5, /* douze */
    6, /* treize*/
    8, /* quatorze */
    6, /* quinze   */
    5, /* seize    */
    7, /* dixsept  */
    7, /* dixhuit  */
    7  /* dixneuf  */
};

/* Lengths of the basic tens words:
   index = 2 → "vingt", 3 → "trente" etc. */
static const int tens_len[10] = {
    0, 0, 5, /* vingt  */
    6, /* trente */
    8, /* quarante */
    9, /* cinquante */
    8  /* soixante */
};

static inline seq_t french_len(idx_t n) {
    if (n == 0) return (seq_t)4;          /* "zéro" */
    if (n < 20) return (seq_t)unit_len[n];

    /* 20‑99 */
    if (n < 100) {
        int tens = n / 10;
        int rem = n % 10;

        /* 70‑79 and 90‑99 handled separately */
        if (tens == 7) {                /* 70‑79 */
            int r = n - 60;             /* r is 10‑19 */
            int base = 8;               /* "soixante" */
            if (r == 10) return (seq_t)(base + 3);     /* dix */
            if (r == 11) return (seq_t)(base + 2 + unit_len[11]); /* et onze */
            return (seq_t)(base + unit_len[r]);          /* 12‑19 */
        }
        if (tens == 9) {                /* 90‑99 */
            int r = n - 90;             /* r is 0‑9 */
            int base = 11;              /* "quatrevingt" (without s) */
            if (r == 0) return (seq_t)(base + 3);       /* dix */
            if (r == 1) return (seq_t)(base + unit_len[11]); /* onze */
            return (seq_t)(base + unit_len[10 + r - 2]);     /* 12‑19 */
        }

        /* 20‑69, 80‑89 (handled separately below) */
        if (tens == 8) {                /* 80‑89 */
            int r = n - 80;             /* 0‑9 */
            int base = 11;              /* "quatrevingt" */
            if (r == 0) return (seq_t)(base + 1);         /* plural s */
            return (seq_t)(base + unit_len[r]);           /* 1‑9 */
        }

        /* 20‑69 */
        if (rem == 0) return (seq_t)tens_len[tens];
        if (rem == 1 && (tens==2||tens==3||tens==4||tens==5||tens==6))
            return (seq_t)(tens_len[tens] + 2 + unit_len[1]); /* et un */
        return (seq_t)(tens_len[tens] + unit_len[rem]);
    }

    /* 100‑999 */
    if (n < 1000) {
        int hundreds = n / 100;
        int rem = n % 100;
        int len = 0;
        if (hundreds > 0) {
            int cent_len = 4;                 /* "cent" */
            if (rem == 0 && hundreds > 1) cent_len = 5;  /* "cents" */
            len += unit_len[hundreds] + cent_len;
        }
        if (rem > 0) len += (int)french_len((idx_t)rem);
        return (seq_t)len;
    }

    /* 1000 and above */
    int thousands = n / 1000;
    int rem = n % 1000;
    int len;
    if (thousands == 1) {
        len = 5; /* "mille" */
    } else {
        len = (int)french_len((idx_t)thousands) + 5; /* e.g., "deux mille" */
    }
    if (rem > 0) len += (int)french_len((idx_t)rem);
    return (seq_t)len;
}

seq_t a(idx_t n) {
    return french_len(n);
}
