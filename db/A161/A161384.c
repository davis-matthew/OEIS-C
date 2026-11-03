/*
 * A161384  The numbers 0-100 in Spanish lexicographic order.
 * https://oeis.org/A161384
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn, word
 * Formula:   a(n) = the nth number among 0..100 sorted by lexicographic order
 *            of their Spanish names
 * Range:     u8 n <= 101, u16 n <= 101, u32 n <= 101, u64 n <= 101,
 *            u128 n <= 101, big any n
 * Validated: full, pass, 101 terms through n = 101 (2026-11-02)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>

/* Helper: write the Spanish name of number n (0 <= n <= 100) into buf */
static void write_spanish(idx_t n, char *buf, size_t bufsz)
{
    static const char *ones[10] = {
        "", "uno", "dos", "tres", "cuatro", "cinco",
        "seis", "siete", "ocho", "nueve"
    };

    static const char *teen[4] = { /* 16..19 */
        "dieciseis", "diecisiete", "dieciocho", "diecinueve"
    };

    static const char *veinti[9] = { /* 21..29 */
        "veintiuno", "veintidos", "veintitres",
        "veinticuatro", "veinticinco", "veintiseis",
        "veintisiete", "veintiocho", "veintinueve"
    };

    if (n == 0) {
        strncpy(buf, "cero", bufsz); return;
    }
    if (n <= 15) {
        static const char *ones15[16] = {
            "", "uno", "dos", "tres", "cuatro",
            "cinco", "seis", "siete", "ocho", "nueve",
            "diez", "once", "doce", "trece", "catorce",
            "quince"
        };
        strncpy(buf, ones15[n], bufsz); return;
    }
    if (n >= 16 && n <= 19) {
        strncpy(buf, teen[n-16], bufsz); return;
    }
    if (n == 20) { strncpy(buf, "veinte", bufsz); return; }
    if (n >= 21 && n <= 29) {
        strncpy(buf, veinti[n-21], bufsz); return;
    }
    if (n == 30) { strncpy(buf, "treinta", bufsz); return; }
    if (n >= 31 && n <= 39) {
        snprintf(buf, bufsz, "treinta y %s", ones[(size_t)(n-30)]); return;
    }
    if (n == 40) { strncpy(buf, "cuarenta", bufsz); return; }
    if (n >= 41 && n <= 49) {
        snprintf(buf, bufsz, "cuarenta y %s", ones[(size_t)(n-40)]); return;
    }
    if (n == 50) { strncpy(buf, "cincuenta", bufsz); return; }
    if (n >= 51 && n <= 59) {
        snprintf(buf, bufsz, "cincuenta y %s", ones[(size_t)(n-50)]); return;
    }
    if (n == 60) { strncpy(buf, "sesenta", bufsz); return; }
    if (n >= 61 && n <= 69) {
        snprintf(buf, bufsz, "sesenta y %s", ones[(size_t)(n-60)]); return;
    }
    if (n == 70) { strncpy(buf, "setenta", bufsz); return; }
    if (n >= 71 && n <= 79) {
        snprintf(buf, bufsz, "setenta y %s", ones[(size_t)(n-70)]); return;
    }
    if (n == 80) { strncpy(buf, "ochenta", bufsz); return; }
    if (n >= 81 && n <= 89) {
        snprintf(buf, bufsz, "ochenta y %s", ones[(size_t)(n-80)]); return;
    }
    if (n == 90) { strncpy(buf, "noventa", bufsz); return; }
    if (n >= 91 && n <= 99) {
        snprintf(buf, bufsz, "noventa y %s", ones[(size_t)(n-90)]); return;
    }
    if (n == 100) { strncpy(buf, "cien", bufsz); return; }
}

/* Comparator for qsort: compare Spanish names */
static int cmp_spanish(const void *p1, const void *p2)
{
    idx_t a = *(const idx_t *)p1;
    idx_t b = *(const idx_t *)p2;
    char sa[64], sb[64];
    write_spanish(a, sa, sizeof(sa));
    write_spanish(b, sb, sizeof(sb));
    return strcmp(sa, sb);
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* static array holding 0..100 sorted lexicographically */
    static idx_t order[101];
    static bool initialized = false;

    if (!initialized) {
        for (idx_t i = 0; i <= 100; ++i) order[i] = i;
        qsort(order, 101, sizeof(idx_t), cmp_spanish);
        initialized = true;
    }

    if (n == 0) return 0;          /* n is 1-based; undefined below 1 */
    if (n-1 < 101) return (seq_t)order[n-1];
    /* For n > 101 the sequence is undefined; return 0 */
    return 0;
}
