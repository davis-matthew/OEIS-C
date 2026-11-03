/*
 * A160777  Numbers that have an "e" in the middle of their names in Spanish.
 * https://oeis.org/A160777
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, word
 * Formula:   not stated
 * Range:     u8 n <= 24, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Validated: partial, pass, 52 terms through n = 52 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A160777: Numbers whose Spanish name has an “e” in the middle.
   The function searches the natural numbers, keeps the found terms
   in a memoized cache, and returns the n‑th term (1‑based). */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/mem.h"

static void number_to_spanish(uint64_t n, char *buf, size_t *len)
{
    *len = 0;
    char *p = buf;  /* local write pointer */

    /* thousands (up to 999999) */
    if (n >= 1000) {
        uint64_t thousands = n / 1000;
        if (thousands == 1) {
            const char *mil = "mil";
            while (*mil) { *p++ = *mil++; (*len)++; }
        } else {
            char temp[256];
            size_t temp_len = 0;
            number_to_spanish(thousands, temp, &temp_len);
            for (size_t i = 0; i < temp_len; i++) { *p++ = temp[i]; (*len)++; }
            const char *mil = "mil";
            while (*mil) { *p++ = *mil++; (*len)++; }
        }
        n %= 1000;
    }

    /* hundreds */
    if (n >= 100) {
        if (n == 100) {
            const char *cien = "cien";
            while (*cien) { *p++ = *cien++; (*len)++; }
        } else {
            uint64_t h = n / 100;
            if (h == 1) {
                const char *ciento = "ciento";
                while (*ciento) { *p++ = *ciento++; (*len)++; }
            } else {
                static const char *hundreds[8] = {
                    "doscientos", "trescientos", "cuatrocientos",
                    "quinientos", "seiscientos", "setecientos",
                    "ochocientos", "novecientos"
                };
                const char *hd = hundreds[h - 2];
                while (*hd) { *p++ = *hd++; (*len)++; }
            }
        }
        n %= 100;
    }

    /* tens and units */
    if (n >= 20) {
        if (n < 30) { /* 20‑29: special forms */
            static const char *special[10] = {
                "veinte", "veintiuno", "veintidos", "veintitres",
                "veinticuatro", "veinticinco", "veintiseis",
                "veintisiete", "veintiocho", "veintinueve"
            };
            const char *sp = special[n - 20];
            while (*sp) { *p++ = *sp++; (*len)++; }
        } else { /* 30‑99 */
            static const char *tens[10] = {
                "", "", "veinte", "treinta", "cuarenta",
                "cincuenta", "sesenta", "setenta",
                "ochenta", "noventa"
            };
            const char *t = tens[n / 10];
            while (*t) { *p++ = *t++; (*len)++; }
            if (n % 10) {
                const char *y = "y";
                while (*y) { *p++ = *y++; (*len)++; }
                static const char *ones[10] = {
                    "", "uno", "dos", "tres", "cuatro",
                    "cinco", "seis", "siete", "ocho", "nueve"
                };
                const char *o = ones[n % 10];
                while (*o) { *p++ = *o++; (*len)++; }
            }
        }
    } else if (n > 0) { /* 1‑19 */
        static const char *ones_full[20] = {
            "", "uno", "dos", "tres", "cuatro", "cinco",
            "seis", "siete", "ocho", "nueve",
            "diez", "once", "doce", "trece", "catorce",
            "quince", "dieciseis", "diecisiete",
            "dieciocho", "diecinueve"
        };
        const char *o = ones_full[n];
        while (*o) { *p++ = *o++; (*len)++; }
    }
}

static int has_middle_e(uint64_t n)
{
    char buf[512];
    size_t len;
    number_to_spanish(n, buf, &len);
    if (len == 0) return 0;
    if (len & 1) { /* odd length only */
        return buf[len / 2] == 'e';
    }
    return 0;
}

/* ---------- helper to build a Spanish name ---------- */

/* ---------- test if a number has an 'e' in the centre of its Spanish name ---------- */

/* ---------- memoised cache for the sequence ---------- */

static seq_t *cache = NULL;
static size_t cache_size = 0;
static size_t cache_cap  = 0;
static uint64_t next_to_check = 1;

static void ensure_cache_capacity(size_t need)
{
    if (cache_size >= need) return;
    if (cache_cap < need) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        if (new_cap < need) new_cap = need;
        seq_t *tmp = mem_resize(seq_t, cache, new_cap);
        if (!tmp) exit(EXIT_FAILURE);
        cache = tmp;
        cache_cap = new_cap;
    }
}

/* ---------- main sequence function ---------- */

seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* offset is 1 */
    while (cache_size < (size_t)n) {
        seq_t val = (seq_t)next_to_check;
        if (has_middle_e(next_to_check)) {
            ensure_cache_capacity(cache_size + 1);
            cache[cache_size++] = val;
        }
        next_to_check++;
    }
    return cache[n - 1];
}
