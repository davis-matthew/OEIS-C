/*
 * A091390  Aronson's mod 16 sequence: "T is the first, fourth, eleventh,
 *          zeroth, ... letter in this sentence, not counting spaces or commas
 *          and all mod 16".
 * https://oeis.org/A091390
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, word
 * Formula:   a(n) = position modulo 16 of the nth 'T' in the * recursively
 *            generated sentence * * (implemented by incremental construction
 *            of the sentence * and scanning the positions of 'T'/'t'
 *            characters)
 * Range:     u8 n <= 88, u16 n <= 88, u32 n <= 88, u64 n <= 88, u128 n <= 88,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Singly‑indexed English ordinal words for 0 … 15 */
static const char *ordinal[] = {
    "zeroth",  "first",   "second",  "third",   "fourth",
    "fifth",   "sixth",   "seventh", "eighth",  "ninth",
    "tenth",   "eleventh","twelfth", "thirteenth","fourteenth",
    "fifteenth"
};

/* Dynamic array for the computed sequence */
static seq_t *cache = NULL;
static size_t cache_size = 0;
static size_t cache_cap  = 0;

/* Buffer holding the concatenated letters of the sentence */
static char *text  = NULL;
static size_t text_len  = 0;
static size_t text_cap  = 0;

/* Positions processed so far in the text buffer */
static size_t processed    = 0;

/* Index of the next ordinal word to append */
static size_t next_ordinal = 0;

/* Flag indicating whether the one‑time initialization has been done */
static bool initialized = false;

/* Ensure the cache has room for at least `need` elements */
static void ensure_cache_capacity(size_t need) {
    if (need <= cache_cap) return;
    size_t new_cap = cache_cap ?: 16;
    while (new_cap < need) new_cap <<= 1;
    cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache_cap = new_cap;
}

/* Ensure the text buffer has room for at least `need` characters */
static void ensure_text_capacity(size_t need) {
    if (need <= text_cap) return;
    size_t new_cap = text_cap ?: 64;
    while (new_cap < need) new_cap <<= 1;
    text = (char *)realloc(text, new_cap);
    text_cap = new_cap;
}

/* Append a NUL‑terminated string of letters to the text buffer */
static void append_letters(const char *s) {
    while (*s) {
        char c = *s++;
        ensure_text_capacity(text_len + 1);
        text[text_len++] = c;
    }
}

/* Main sequence function */
seq_t a(idx_t n) {
    /* One‑time startup: build the initial fixed prefix
       "T is the" (letters only) and prepare state. */
    if (!initialized) {
        initialized = true;
        ensure_cache_capacity(16);
        ensure_text_capacity(64);
        /* Add "T is the" -> "Tis"+"the" */
        append_letters("Tis");
        append_letters("the");
        /* No terms have been extracted yet; processed==0. */
    }

    /* Grow the cache as needed */
    ensure_cache_capacity(cache_size + 1);

    /* Produce terms until we have at least `n + 1` entries */
    while (cache_size <= (size_t)n) {
        /* Scan newly added letters for 'T'/'t' */
        for (; processed < text_len; ++processed) {
            char c = text[processed];
            if (c == 't' || c == 'T') {
                cache[cache_size++] = (seq_t)((processed + 1) % 16);
            }
        }

        /* If more terms still required, append the next ordinal word. */
        if (cache_size <= (size_t)n) {
            /* The ordinal word corresponds to the value of term `next_ordinal`. */
            unsigned val = (unsigned)(cache[next_ordinal]);  /* value fits 0‑15 */
            const char *ord = ordinal[val];
            append_letters(ord);
            next_ordinal++;
        }
    }

    return cache[n];
}
