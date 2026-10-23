/*
 * A160783  Numbers that have a "u" in the middle of their names in Spanish.
 * https://oeis.org/A160783
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, word
 * Formula:   a(n) = nth positive integer whose Spanish concatenated name has
 *            an odd length and the middle character is 'u'
 * Range:     u8 n <= 7, u16 n <= 25, u32 n <= 25, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Spanish words used in the concatenated form.  All letters are
   lower‑case and accents are omitted. */
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

/* Units and numbers smaller than 20. */
static const char *const units[20] = {
    "cero","uno","dos","tres","cuatro","cinco","seis","siete",
    "ocho","nueve","diez","once","doce","trece","catorce",
    "quince","dieciseis","diecisiete","dieciocho","diecinueve"
};

/* Tens words from twenty to ninety. */
static const char *const tens_words[10] = {
    "","", "veinte","treinta","cuarenta","cincuenta","sesenta",
    "setenta","ochenta","noventa"
};

/* Hundred words from two hundred to nine hundred.  The entry for
   one hundred is handled separately. */
static const char *const hundreds_words[10] = {
    "","cien","doscientos","trescientos","cuatrocientos",
    "quinientos","seiscientos","setecientos","ochocientos","novecientos"
};

/* Simple helper to append a C string to a buffer. */
static void append(const char *s, char *buf, size_t *p)
{
    while (*s) buf[(*p)++] = *s++;
}

/* Build the concatenated form of a number < 100. */
static void build_lower_than_100(uint32_t n, char *buf, size_t *p)
{
    if (n < 20) {
        append(units[n], buf, p);
    } else {
        uint32_t tens = n / 10 * 10;
        uint32_t unit = n % 10;
        if (tens == 20 && unit > 0) {            /* 21–29 */
            append("veinti", buf, p);
            append(units[unit], buf, p);
        } else {                                 /* 20,30,…,90, and with units */
            append(tens_words[tens / 10], buf, p);
            if (unit > 0) {
                append("y", buf, p);
                append(units[unit], buf, p);
            }
        }
    }
}

/* Build the concatenated form of a number < 1000. */
static void build_lower_than_1000(uint32_t n, char *buf, size_t *p)
{
    if (n >= 100) {
        uint32_t h = n / 100;
        uint32_t rem = n % 100;
        if (h == 1) {
            if (rem == 0) {
                append("cien", buf, p);
            } else {
                append("ciento", buf, p);
            }
        } else {
            append(hundreds_words[h], buf, p);
        }
        if (rem > 0) {
            build_lower_than_100(rem, buf, p);
        }
    } else {
        build_lower_than_100(n, buf, p);
    }
}

/* Build the concatenated Spanish name of any unsigned 64‑bit number. */
static void build_number(uint64_t n, char *buf, size_t *len)
{
    size_t pos = 0;

    if (n >= 1000000) {                     /* millions */
        uint32_t millions = (uint32_t)(n / 1000000);
        uint64_t rem = n % 1000000;

        if (millions == 1) {                 /* "un millón" */
            append("un", buf, &pos);
            append("millon", buf, &pos);
        } else {                             /* e.g., "dos millones" */
            build_lower_than_1000(millions, buf, &pos);
            append("millones", buf, &pos);
        }

        if (rem > 0) {                       /* thousands and units */
            uint32_t thousands = (uint32_t)(rem / 1000);
            uint32_t rest = (uint32_t)(rem % 1000);

            if (thousands > 0) {
                if (thousands == 1) {
                    append("mil", buf, &pos);
                } else {
                    build_lower_than_1000(thousands, buf, &pos);
                    append("mil", buf, &pos);
                }
            }
            if (rest > 0) {
                build_lower_than_1000(rest, buf, &pos);
            }
        }
    } else if (n >= 1000) {                 /* thousands */
        uint32_t thousands = (uint32_t)(n / 1000);
        uint32_t rest = (uint32_t)(n % 1000);

        if (thousands == 1) {
            append("mil", buf, &pos);
        } else {
            build_lower_than_1000(thousands, buf, &pos);
            append("mil", buf, &pos);
        }
        if (rest > 0) {
            build_lower_than_1000(rest, buf, &pos);
        }
    } else {                                 /* < 1000 */
        build_lower_than_1000((uint32_t)n, buf, &pos);
    }

    *len = pos;
}

/* Main sequence function – returns the n‑th element of A160783. */
seq_t a(idx_t n)
{
    idx_t found = 0;
    for (uint64_t cand = 1; ; ++cand) {
        char buf[256];
        size_t len;
        build_number(cand, buf, &len);

        if ((len & 1U) && buf[len / 2] == 'u') {
            ++found;
            if (found == n) {
                return (seq_t)cand;
            }
        }
    }
}
