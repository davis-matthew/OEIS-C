/*
 * A117383  Positive numbers whose Spanish names do not include repeated
 *          letters.
 * https://oeis.org/A117383
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn, word
 * Formula:   a(n) is the nth positive integer whose Spanish name contains no
 *            repeated letters
 * Range:     u8 n <= 10, u16 n <= 24, u32 n <= 25, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed by enumerating natural numbers and testing the unique‑letter property. */

#include <stdlib.h>
#include <stdbool.h>

/* Mapping of numbers to their Spanish word representations without accents. */
static const char *const unit_words[20] = {
    "cero","uno","dos","tres","cuatro","cinco",
    "seis","siete","ocho","nueve","diez","once",
    "doce","trece","catorce","quince","dieciseis",
    "diecisiete","dieciocho","diecinueve"
};

static const char *const twenty_words[10] = {
    "veinte","veintiuno","veintidos","veintitres",
    "veinticuatro","veinticinco","veintiseis",
    "veintisiete","veintiocho","veintinueve"
};

static const char *const tens_words[10] = {
    NULL,NULL,NULL,"treinta","cuarenta","cincuenta",
    "sesenta","setenta","ochenta","noventa"
};

static const char *const hundreds_words[8] = {
    "doscientos","trescientos","cuatrocientos","quinientos",
    "seiscientos","setecientos","ochocientos","novecientos"
};

/* Adds the word to the letter mask, returning false if a repeated letter occurs. */
static bool add_word(const char *w, unsigned int *mask) {
    for (const char *p = w; *p; ++p) {
        char c = *p;
        if (c == ' ') continue;
        if (c >= 'a' && c <= 'z') {
            unsigned int bit = c - 'a';
            if (*mask & (1u << bit)) return false;
            *mask |= (1u << bit);
        }
    }
    return true;
}

/* Recursively processes a number into its Spanish representation, updating the mask. */
static bool process_idx(idx_t n, unsigned int *mask) {
    if (n < 20) {
        return add_word(unit_words[n], mask);
    } else if (n < 30) {
        return add_word(twenty_words[n - 20], mask);
    } else if (n < 100) {
        int tens = n / 10;
        int ones = n % 10;
        if (!add_word(tens_words[tens], mask)) return false;
        if (ones) return add_word(unit_words[ones], mask);
        return true;
    } else if (n < 1000) {
        int hundreds = n / 100;
        int rem = n % 100;
        if (hundreds == 1) {
            if (rem == 0) {
                return add_word("cien", mask);
            } else {
                if (!add_word("ciento", mask)) return false;
                return process_idx(rem, mask);
            }
        } else {
            if (!add_word(hundreds_words[hundreds - 2], mask)) return false;
            if (rem) return process_idx(rem, mask);
            return true;
        }
    } else {            /* thousands, including higher multiples of 1000 */
        idx_t thousands = n / 1000;
        int rem = n % 1000;
        if (thousands == 1) {
            if (!add_word("mil", mask)) return false;
        } else {
            if (!process_idx(thousands, mask)) return false;
            if (!add_word("mil", mask)) return false;
        }
        if (rem) return process_idx(rem, mask);
        return true;
    }
}

/* Checks if the Spanish representation of n has no repeated letters. */
static bool has_unique_letters_idx(idx_t n) {
    unsigned int mask = 0;
    return process_idx(n, &mask);
}

/* Cached sequence storage */
static seq_t *seq_cache = NULL;
static size_t seq_cache_cap = 0;
static size_t seq_cache_cnt = 0;
static idx_t next_candidate = 1;

seq_t a(idx_t n) {
    if (!seq_cache) {
        seq_cache_cap = 128;
        seq_cache = (seq_t *)malloc(seq_cache_cap * sizeof(seq_t));
    }
    while (seq_cache_cnt < n) {
        if (has_unique_letters_idx(next_candidate)) {
            if (seq_cache_cnt == seq_cache_cap) {
                seq_cache_cap *= 2;
                seq_t *tmp = (seq_t *)realloc(seq_cache,
                                              seq_cache_cap * sizeof(seq_t));
                if (!tmp) exit(1);
                seq_cache = tmp;
            }
            seq_cache[seq_cache_cnt++] = (seq_t)next_candidate;
        }
        ++next_candidate;
    }
    return seq_cache[n - 1];
}
