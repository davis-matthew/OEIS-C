/*
 * A160776  Numbers that have a "c" in the middle of their names in Spanish.
 * https://oeis.org/A160776
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, word
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 31, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Computes a(n) of OEIS A160776:
    the n‑th positive integer whose Spanish name (no spaces, no accents)
    contains a 'c' or 'C' exactly in the middle of its characters.  */

/*  The modern routine works in C11 and C++17.  It caches found
    terms and can be called in any order.  For C it uses malloc/realloc,
    for C++ it uses std::vector.  */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#ifdef __cplusplus
#include <vector>
#endif

/* ------------------------------------------------------------------ */
/*  Helpers for writing a Spanish name (no spaces, no accents).       */
/* ------------------------------------------------------------------ */

static void append_str(char *buf, size_t *pos, const char *s)
{
    while (*s) buf[(*pos)++] = *s++;
}

/* write a group of at most 3 decimal digits (value < 1000) into buf   */
static void write_group(uint64_t g, char *buf, size_t *pos)
{
    if (g == 0) return;

    static const char *hundreds_names[10] =
        { "", "", "doscientos", "trescientos", "cuatrocientos",
          "quinientos", "seiscientos", "setecientos", "ochocientos",
          "novecientos" };

    static const char *units_0_19[20] =
        { "", "uno", "dos", "tres", "cuatro", "cinco",
          "seis", "siete", "ocho", "nueve", "diez",
          "once", "doce", "trece", "catorce", "quince",
          "dieciseis", "diecisiete", "dieciocho", "diecinueve" };

    static const char *tens_names[10] =
        { "", "", "veinte", "treinta", "cuarenta", "cincuenta",
          "sesenta", "setenta", "ochenta", "noventa" };

    static const char *units_1_9[10] =
        { "", "uno", "dos", "tres", "cuatro", "cinco",
          "seis", "siete", "ocho", "nueve" };

    uint64_t hundreds = g / 100;
    uint64_t rest     = g % 100;

    if (hundreds) {
        if (hundreds == 1) {
            if (rest == 0)
                append_str(buf, pos, "cien");
            else
                append_str(buf, pos, "ciento");
        } else {
            append_str(buf, pos, hundreds_names[hundreds]);
        }
    }

    if (rest) {
        if (rest < 20) {
            append_str(buf, pos, units_0_19[rest]);
        } else {
            uint64_t tens = rest / 10;
            uint64_t unit = rest % 10;
            if (tens == 2) {
                if (unit == 0)
                    append_str(buf, pos, "veinte");
                else {
                    append_str(buf, pos, "veinti");
                    append_str(buf, pos, units_1_9[unit]);
                }
            } else {
                append_str(buf, pos, tens_names[tens]);
                if (unit) {
                    append_str(buf, pos, "y");
                    append_str(buf, pos, units_1_9[unit]);
                }
            }
        }
    }
}

/* Convert integer n (>=1) to Spanish number written without spaces   */
static size_t write_spanish_name(uint64_t n, char *buf)
{
    uint64_t groups[10];
    int      group_cnt = 0;

    while (n) {
        groups[group_cnt++] = n % 1000;
        n /= 1000;
    }

    size_t pos = 0;
    for (int i = group_cnt - 1; i >= 0; --i) {
        uint64_t g = groups[i];
        if (!g) continue;

        if (i == 0) {                      /* units */
            write_group(g, buf, &pos);
        } else if (i == 1) {               /* thousand */
            if (g == 1)
                append_str(buf, &pos, "mil");
            else {
                write_group(g, buf, &pos);
                append_str(buf, &pos, "mil");
            }
        } else if (i == 2) {               /* million */
            if (g == 1)
                append_str(buf, &pos, "un millón");
            else {
                write_group(g, buf, &pos);
                append_str(buf, &pos, "millones");
            }
        } else if (i == 3) {               /* thousand millions (miliard) */
            if (g == 1)
                append_str(buf, &pos, "mil millones");
            else {
                write_group(g, buf, &pos);
                append_str(buf, &pos, "mil millones");
            }
        } else if (i == 4) {               /* trillón (billón) */
            if (g == 1)
                append_str(buf, &pos, "un billón");
            else {
                write_group(g, buf, &pos);
                append_str(buf, &pos, "billones");
            }
        } else {                           /* higher groups – just number */
            write_group(g, buf, &pos);
        }
    }
    return pos;
}

/* check if a number satisfies the property */
static bool qualifies(uint64_t n)
{
    char   buf[2048];
    size_t len = write_spanish_name(n, buf);
    if (len % 2 == 0) return false;        /* must be odd length */
    char mid = buf[len / 2];
    return (mid == 'c' || mid == 'C');
}

/* ------------------------------------------------------------------ */
/*  Public function a(idx_t n)                                        */
/* ------------------------------------------------------------------ */

#if defined(__cplusplus)
static std::vector<seq_t> cache_vec;
static uint64_t next_to_test_vec = 1;
#else
static seq_t *cache = NULL;
static size_t cached_cap = 0;
static size_t cached_cnt = 0;
static uint64_t next_to_test = 1;
#endif

extern seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0; /* usage outside offset is irrelevant */

#if defined(__cplusplus)
    /* C++ mode – use std::vector */
    while (cache_vec.size() < (size_t)n) {
        while (!qualifies(next_to_test_vec)) ++next_to_test_vec;
        cache_vec.push_back((seq_t)next_to_test_vec);
        ++next_to_test_vec;
    }
    return cache_vec[(size_t)n - 1];
#else
    /* C mode – use malloc/realloc */
    while (cached_cnt < (size_t)n) {
        while (!qualifies(next_to_test)) ++next_to_test;

        /* grow array if necessary */
        if (cached_cap <= cached_cnt) {
            size_t new_cap = cached_cap ? cached_cap * 2 : 64;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) abort();           /* allocation failure */
            cache = tmp;
            cached_cap = new_cap;
        }

        cache[cached_cnt++] = (seq_t)next_to_test;
        ++next_to_test;
    }
    return cache[(size_t)n - 1];
#endif
}
