/*
 * A163321  Numbers whose Spanish names include all five vowels exactly once.
 * https://oeis.org/A163321
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, nonn, word
 * Formula:   a(n) = numbers whose Spanish names contain all five vowels a, e,
 *            i, o, u exactly once
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   vowel_index
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation is an incremental search that masks vowel usage. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/vowel_index.h"

/* Basic word parts for numbers. */
static const char *const units_str[10] = {
    "cero", "uno", "dos", "tres", "cuatro", "cinco",
    "seis", "siete", "ocho", "nueve"
};

static const char *const teens_str[10] = {
    "diez", "once", "doce", "trece", "catorce",
    "quince", "dieciseis", "diecisiete", "dieciocho", "diecinueve"
};

static const char *const tens_str[10] = {
    NULL, NULL, "veinte", "treinta", "cuarenta",
    "cincuenta", "sesenta", "setenta", "ochenta", "noventa"
};

static const char *const hundreds_str[10] = {
    NULL, NULL, "doscientos", "trescientos", "cuatrocientos",
    "quinientos", "seiscientos", "setecientos",
    "ochocientos", "novecientos"
};

/* Add the vowels of a word to the mask. Returns false if a duplicate vowel
   is detected. */
static bool add_word(const char *w, uint8_t *mask)
{
    for (const char *p = w; *p; ++p) {
        int idx = vowel_index(*p);
        if (idx >= 0) {
            if (*mask & (1u << idx))
                return false;                /* duplicate vowel */
            *mask |= (1u << idx);
        }
    }
    return true;
}

/* Recursive conversion of a number < 1000 to its Spanish form (mask). */
static bool process_0_to_999(uint16_t x, uint8_t *mask, bool use_special_un)
{
    if (x == 0)
        return true;

    if (x < 10) {
        return add_word(units_str[x], mask);
    }
    if (x < 20) {
        return add_word(teens_str[x - 10], mask);
    }
    if (x < 30) {          /* 20–29 */
        if (x == 20)
            return add_word("veinte", mask);
        if (!add_word("veinti", mask))
            return false;
        return process_0_to_999(x - 20, mask, false);
    }
    if (x < 100) {         /* 30–99 */
        unsigned t = x / 10u;
        unsigned u = x % 10u;
        if (!add_word(tens_str[t], mask))
            return false;
        if (u) {
            if (!add_word("y", mask))
                return false;
            if (use_special_un && u == 1) {
                if (!add_word("un", mask))
                    return false;
            } else {
                if (!add_word(units_str[u], mask))
                    return false;
            }
        }
        return true;
    }
    /* 100–999 */
    unsigned h = x / 100u;
    unsigned r = x % 100u;
    if (h == 1 && r == 0) {
        return add_word("cien", mask);
    }
    if (h == 1) {
        if (!add_word("ciento", mask))
            return false;
        return process_0_to_999(r, mask, false);
    }
    if (!add_word(hundreds_str[h], mask))
        return false;
    return process_0_to_999(r, mask, false);
}

/* Process the whole number, taking into account thousands/millions/billions. */
static bool process_number(uint64_t n, uint8_t *mask)
{
    uint16_t groups[7];
    size_t gcnt = 0;
    while (n > 0) {
        groups[gcnt++] = (uint16_t)(n % 1000u);
        n /= 1000u;
    }
    for (int i = (int)gcnt - 1; i >= 0; --i) {
        uint16_t g = groups[i];
        if (g == 0)
            continue;
        switch (i) {
            case 1: /* thousands */
                if (g == 1) {
                    if (!add_word("mil", mask))
                        return false;
                } else {
                    if (!process_0_to_999(g, mask, true))
                        return false;
                    if (!add_word("mil", mask))
                        return false;
                }
                break;
            case 2: /* millions */
                if (g == 1) {
                    if (!add_word("un", mask))
                        return false;
                    if (!add_word("millon", mask))
                        return false;
                } else {
                    if (!process_0_to_999(g, mask, false))
                        return false;
                    if (!add_word("millones", mask))
                        return false;
                }
                break;
            case 3: /* billions */
                if (g == 1) {
                    if (!add_word("mil", mask))
                        return false;
                    if (!add_word("millones", mask))
                        return false;
                } else {
                    if (!process_0_to_999(g, mask, false))
                        return false;
                    if (!add_word("mil", mask))
                        return false;
                    if (!add_word("millones", mask))
                        return false;
                }
                break;
            default:
                if (!process_0_to_999(g, mask, false))
                    return false;
                break;
        }
    }
    return true;
}

static bool has_all_unique_vowels(uint64_t n)
{
    uint8_t mask = 0;
    if (!process_number(n, &mask))
        return false;
    return mask == 0x1Fu; /* a,e,i,o,u set */
}

/* Storage for the sequence values. */
static seq_t *seq = NULL;
static size_t seq_cap = 0;
static idx_t seq_len = 0;
static uint64_t next_candidate = 1;

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0; /* undefined, but safe */

    while (seq_len < (size_t)n) {
        while (!has_all_unique_vowels(next_candidate))
            ++next_candidate;

        if (seq_len == seq_cap) {
            seq_cap = (seq_cap) ? seq_cap * 2 : 16;
            seq = (seq_t*)realloc(seq, seq_cap * sizeof(seq_t));
        }
        seq[seq_len++] = (seq_t)next_candidate;
        ++next_candidate;
    }
    return seq[seq_len - 1];
}
