/*
 * A160781  Numbers that have an "r" in the middle of their names in Spanish.
 * https://oeis.org/A160781
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, word
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 14, u64 n <= 14, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n‑th term of OEIS A160781 by brute‑force search of Spanish
 * number names and checking whether the middle letter of the concatenated
 * name (without spaces or accents) is 'r'. */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

static const char *UNITS[20] = {
    "", "uno", "dos", "tres", "cuatro", "cinco",
    "seis", "siete", "ocho", "nueve", "diez",
    "once", "doce", "trece", "catorce", "quince",
    "dieciseis", "diecisiete", "dieciocho", "diecinueve"
};

static const char *TENS[10] = {
    "", "", "", "treinta", "cuarenta", "cincuenta",
    "sesenta", "setenta", "ochenta", "noventa"
};

static const char *HUNDREDS[10] = {
    "", "", "doscientos", "trescientos", "cuatrocientos",
    "quinientos", "seiscientos", "setecientos",
    "ochocientos", "novecientos"
};

static inline void write_word(const char *w, size_t *len, char *buf)
{
    for (const char *p = w; *p; ++p)
        buf[(*len)++] = *p;
}

static void write_tens_unit(uint64_t v, size_t *len, char *buf)
{
    if (v == 0) return;
    if (v == 20) {
        write_word("veinte", len, buf);
    } else if (v < 20) {
        write_word(UNITS[v], len, buf);
    } else if (v < 30) {           /* 21‑29 */
        write_word("veinti", len, buf);
        write_word(UNITS[v - 20], len, buf);
    } else {
        uint64_t tens = v / 10;
        uint64_t unit = v % 10;
        write_word(TENS[tens], len, buf);
        if (unit > 0) {
            write_word("y", len, buf);
            write_word(UNITS[unit], len, buf);
        }
    }
}

static void write_number(uint64_t x, size_t *len, char *buf)
{
    if (x == 0) return;
    if (x >= 100) {
        if (x == 100) {
            write_word("cien", len, buf);
            return;
        }
        uint64_t h = x / 100;
        uint64_t rest = x % 100;
        if (h == 1) {
            write_word("ciento", len, buf);
        } else {
            write_word(HUNDREDS[h], len, buf);
        }
        if (rest > 0) {
            write_tens_unit(rest, len, buf);
        }
    } else {
        write_tens_unit(x, len, buf);
    }
}

static void write_spanish(uint64_t n, size_t *len, char *buf)
{
    if (n >= 1000) {
        uint64_t thousands = n / 1000;
        uint64_t rest = n % 1000;
        if (thousands == 1) {
            write_word("mil", len, buf);
        } else {
            write_number(thousands, len, buf);
            write_word("mil", len, buf);
        }
        if (rest > 0) {
            write_number(rest, len, buf);
        }
    } else {
        write_number(n, len, buf);
    }
}

static bool has_middle_r(uint64_t n)
{
    char buf[300];
    size_t l = 0;
    write_spanish(n, &l, buf);
    return (l & 1) && buf[l / 2] == 'r';
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /* offset is 1, undefined for n<1 */
    uint64_t found = 0;
    uint64_t count = 0;
    while (count < n) {
        ++found;
        if (has_middle_r(found))
            ++count;
    }
    return (seq_t)found;
}
