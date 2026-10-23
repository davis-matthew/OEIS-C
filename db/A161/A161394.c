/*
 * A161394  Numbers divisible by the number of letters to express them in
 *          Spanish.
 * https://oeis.org/A161394
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, word
 * Formula:   not stated
 * Range:     u8 n <= 29, u16 n <= 40, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   ensure_capacity
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A161394: a(n) = the n-th natural number that is divisible by the
   number of letters in its Spanish word representation
   (letters only, ignoring spaces, hyphens and accents). */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/ensure_capacity.h"

/* Lengths for numbers 1–19 (ignoring 0). */
static const unsigned char small_len[20] = {
    0, /* 0 (unused) */
    3, /* 1: uno */
    3, /* 2: dos */
    4, /* 3: tres */
    6, /* 4: cuatro */
    5, /* 5: cinco */
    4, /* 6: seis */
    5, /* 7: siete */
    4, /* 8: ocho */
    5, /* 9: nueve */
    4, /*10: diez */
    4, /*11: once */
    4, /*12: doce */
    5, /*13: trece */
    7, /*14: catorce */
    6, /*15: quince */
    9, /*16: dieciseis */
   10, /*17: diecisiete */
    9, /*18: dieciocho */
   10  /*19: diecinueve */
};

/* Lengths for tens multiples 20–90. */
static const unsigned char tens_len[10] = {
    0, 0, 6, /*20: veinte*/
    7,       /*30: treinta*/
    8,       /*40: cuarenta*/
    9,       /*50: cincuenta*/
    7,       /*60: sesenta*/
    7,       /*70: setenta*/
    7,       /*80: ochenta*/
    7        /*90: noventa*/
};

/* Lengths for hundreds 200–900 (hundred prefixes). */
static const unsigned char hundreds_len[10] = {
    0, 0, /*100 handled separately*/
   10,    /*200: doscientos*/
   11,    /*300: trescientos*/
   13,    /*400: cuatrocientos*/
   10,    /*500: quinientos*/
   11,    /*600: seiscientos*/
   11,    /*700: setecientos*/
   11,    /*800: ochocientos*/
   11     /*900: novecientos*/
};

/* Return the number of letters in the Spanish word of a positive integer n,
   ignoring spaces, hyphens and accents. */
static inline size_t spanish_len(idx_t n)
{
    if (n < 20) return small_len[n];

    if (n < 30) /* 20–29: veinti... */
        return 6 + small_len[n - 20];

    if (n < 100) {
        if (n % 10 == 0)
            return tens_len[n / 10];
        return tens_len[n / 10] + 1 + small_len[n % 10]; /* +1 for 'y' */
    }

    if (n < 1000) {
        idx_t h = n / 100;
        idx_t r = n % 100;
        if (h == 1) {           /* 100–199: cien/ciento */
            if (r == 0)
                return 4;       /* cien */
            return 6 + spanish_len(r); /* ciento + rest */
        } else {
            size_t len = hundreds_len[h];
            if (r > 0)
                len += spanish_len(r);
            return len;
        }
    }

    if (n < 1000000) {           /* thousands */
        idx_t t = n / 1000;
        idx_t r = n % 1000;
        size_t len = (t == 1) ? 3 : spanish_len(t) + 3; /* 'mil' */
        if (r > 0)
            len += spanish_len(r);
        return len;
    }

    if (n < 1000000000) {        /* millions */
        idx_t m = n / 1000000;
        idx_t r = n % 1000000;
        size_t len = (m == 1) ? 8 : spanish_len(m) + 8; /* 'millón(s)' */
        if (r > 0)
            len += spanish_len(r);
        return len;
    }

    /* Numbers beyond 1 000 000 000 are not needed for the expected range. */
    return 0;
}

/* Cache of terms. */
static seq_t *term_cache = NULL;
static size_t term_cap   = 0;
static idx_t  term_count = 0;
static idx_t  next_scan  = 1; /* next number to test for membership */

/* Return the n-th term of A161394. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;        /* defined only for n >= 1 */

    while (term_count < n) {
        size_t len = spanish_len(next_scan);
        if (len > 0 && (next_scan % (idx_t)len == 0)) {
            /* Ensure capacity for one more element. */
            ensure_capacity(term_count + 1, &term_cap,
                            (void **)&term_cache, sizeof(seq_t));
            term_cache[term_count++] = (seq_t)next_scan;
        }
        ++next_scan;
    }
    return term_cache[n - 1];
}
