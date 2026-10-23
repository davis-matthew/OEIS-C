/*
 * A160780  Numbers that have an "o" in the middle of their names in Spanish.
 * https://oeis.org/A160780
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, word
 * Formula:   a(n) = the nth number whose Spanish name has an 'o' in the
 *            middle
 * Range:     u8 n <= 9, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  This function generates Spanish number names (without spaces or
 *   accents) and keeps a cache of the sequence values.  It stops
 *   as soon as the nth value has been found and then returns it.
 *
 *  The "middle" of the name is the central character of the
 *  concatenated string of all words.  The candidate number is
 *  accepted if this character is the letter 'o'.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* Units and special words in Spanish */
static const char *units[20] = {
    "", "uno",  "dos",   "tres",  "cuatro", "cinco",
    "seis",  "siete","ocho",  "nueve",
    "diez",  "once", "doce",  "trece", "catorce",
    "quince","dieciseis","diecisiete","dieciocho","diecinueve"
};

static const char *tens[10] = {
    "", "", "veinte", "treinta", "cuarenta",
    "cincuenta", "sesenta", "setenta",
    "ochenta", "noventa"
};

static const char *hundreds[10] = {
    "", "ciento",  "doscientos",   "trescientos",
    "cuatrocientos","quinientos","seiscientos",
    "setecientos","ochocientos","novecientos"
};

/* Append a word into a buffer pointer */
static void append_word(const char *s, char **p)
{
    while (*s) {
        *(*p)++ = *s++;
    }
}

/* Recursive construction of zero‑space Spanish name for n */
static void spell_nospace(uint64_t n, char **p)
{
    if (n < 10) {
        append_word(units[n], p);
    } else if (n < 20) {
        append_word(units[n], p);
    } else if (n < 30) {
        if (n == 20) {
            append_word("veinte", p);
        } else {                      /* 21‑29 : veinti + unit */
            append_word("veinti", p);
            append_word(units[n - 20], p);
        }
    } else if (n < 100) {
        int t = n / 10;
        int u = n % 10;
        append_word(tens[t], p);
        if (u) {
            append_word("y", p);
            append_word(units[u], p);
        }
    } else if (n < 1000) {
        if (n == 100) {
            append_word("cien", p);
            return;
        }
        int h = n / 100;
        int r = n % 100;
        if (h == 1) {
            append_word("ciento", p);
        } else {
            append_word(hundreds[h], p);
        }
        if (r) {
            spell_nospace(r, p);
        }
    } else if (n < 1000000) {         /* thousands */
        uint64_t th = n / 1000;
        uint64_t r  = n % 1000;
        if (th == 1) {
            append_word("mil", p);
        } else {
            spell_nospace(th, p);
            append_word("mil", p);
        }
        if (r) {
            spell_nospace(r, p);
        }
    } else if (n < 1000000000) {      /* millions */
        uint64_t mi = n / 1000000;
        uint64_t r  = n % 1000000;
        if (mi == 1) {
            append_word("unmillion", p);
        } else {
            spell_nospace(mi, p);
            append_word("millones", p);
        }
        if (r) {
            spell_nospace(r, p);
        }
    } else {                          /* fallback – should not be hit for problem bounds */
        spell_nospace(n % 1000000, p);
    }
}

/* Build the full zero‑space name into buf */
static void spanish_nospace(uint64_t n, char *buf)
{
    char *p = buf;
    spell_nospace(n, &p);
    *p = '\0';
}

/* Test whether a number has an 'o' in the middle of its Spanish name */
static int middle_o(uint64_t n)
{
    char tmp[512];
    spanish_nospace(n, tmp);
    size_t len = strlen(tmp);
    return (len & 1) && (tmp[len/2] == 'o');
}

/* Main sequence function */
seq_t a(idx_t n)
{
    static seq_t *cache   = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static uint64_t candidate = 1;  /* next number to test */

    if ((size_t)n <= cache_len)
        return cache[n-1];

    /* Extend cache until we have the first n terms */
    while (cache_len < (size_t)n) {
        if (middle_o(candidate)) {
            if (cache_len >= cache_cap) {
                cache_cap = cache_cap ? cache_cap * 2 : 16;
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_len++] = (seq_t)candidate;
        }
        ++candidate;
    }

    return cache[n-1];
}
