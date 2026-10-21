/*
 * A059916  Nonnegative integers that when written out in full in English have
 *          no repeated letter.
 * https://oeis.org/A059916
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn, word
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 16, u32 n <= 16, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   compute_mask
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: Generates numbers whose English representation has no repeated letter */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/compute_mask.h"

/* Helper structure for a word: its letter mask and validity */
struct WordEntry { unsigned mask; bool valid; };
static bool masks_initialized = false;

/* Word tables */
static struct WordEntry units[10];
static struct WordEntry teens[10];
static struct WordEntry tens[8];
static struct WordEntry scales[7];    /* 0:"", 1:"thousand", ... 6:"quintillion" */
static struct WordEntry hundred;

/* Initialize all word masks once */
static void init_masks(void)
{
    if (masks_initialized) return;
    const char *unit_str[10]   = {"zero","one","two","three","four","five","six","seven","eight","nine"};
    const char *teen_str[10]   = {"ten","eleven","twelve","thirteen","fourteen",
                                 "fifteen","sixteen","seventeen","eighteen","nineteen"};
    const char *ten_str[8]     = {"twenty","thirty","forty","fifty",
                                  "sixty","seventy","eighty","ninety"};
    const char *scale_str[7]   = {"", "thousand","million","billion",
                                  "trillion","quadrillion","quintillion"};
    const char *hundred_str    = "hundred";

    unsigned m; bool v;
    for (int i = 0; i < 10; ++i) { compute_mask(unit_str[i], &m, &v); units[i].mask = m; units[i].valid = v; }
    for (int i = 0; i < 10; ++i) { compute_mask(teen_str[i], &m, &v); teens[i].mask = m; teens[i].valid = v; }
    for (int i = 0; i < 8;  ++i) { compute_mask(ten_str[i],  &m, &v); tens[i].mask = m; tens[i].valid  = v; }
    for (int i = 0; i < 7;  ++i) { compute_mask(scale_str[i], &m, &v); scales[i].mask = m; scales[i].valid = v; }
    compute_mask(hundred_str, &m, &v); hundred.mask = m; hundred.valid = v;
    masks_initialized = true;
}

/* Build mask for a number 1..999, returning false if invalid (duplicate letters or internal conflict) */
static bool build_mask_group(uint16_t v, unsigned *mask_out)
{
    unsigned m = 0;
    if (v == 0) { *mask_out = 0; return true; }

    /* Hundreds part */
    if (v >= 100) {
        int h = v / 100;
        const struct WordEntry *u = &units[h];
        if (!u->valid || (m & u->mask)) return false;
        m |= u->mask;
        if (!hundred.valid || (m & hundred.mask)) return false;
        m |= hundred.mask;
        v %= 100;
        if (v == 0) { *mask_out = m; return true; }
    }

    /* Tens and units part */
    if (v < 20) {
        const struct WordEntry *w = (v < 10) ? &units[v] : &teens[v-10];
        if (!w->valid || (m & w->mask)) return false;
        m |= w->mask;
    } else {
        int ten = v / 10;          /* 2..9 */
        int unit = v % 10;
        const struct WordEntry *t = &tens[ten-2];
        if (!t->valid || (m & t->mask)) return false;
        m |= t->mask;
        if (unit > 0) {
            const struct WordEntry *u = &units[unit];
            if (!u->valid || (m & u->mask)) return false;
            m |= u->mask;
        }
    }
    *mask_out = m;
    return true;
}

/* Check whether a nonnegative integer has a unique-letter English name */
static bool is_valid_number(uint64_t n)
{
    if (n == 0) return units[0].valid;   /* "zero" */

    uint64_t tmp = n;
    int scale_idx = 0;
    unsigned global_mask = 0;

    while (tmp > 0 || scale_idx == 0) {      /* process groups of three digits */
        uint16_t group = (uint16_t)(tmp % 1000);
        tmp /= 1000;

        if (group != 0) {
            unsigned gmask;
            if (!build_mask_group(group, &gmask)) return false;
            if (global_mask & gmask) return false;
            global_mask |= gmask;

            if (scale_idx >= 1) {
                const struct WordEntry *s = &scales[scale_idx];
                if (!s->valid || (global_mask & s->mask)) return false;
                global_mask |= s->mask;
            }
        }
        /* if group == 0 we skip adding any words (the scale word is omitted) */
        ++scale_idx;
    }
    return true;
}

/* Sequence generator */
seq_t a(idx_t n)
{
    init_masks();

    /* Static storage for generated terms */
    static seq_t *seq = NULL;
    static size_t seq_len = 0, seq_cap = 0;
    static uint64_t cur_val = 0;
    static bool seq_initialized = false;

    if (!seq_initialized) {
        seq_cap = 128;
        seq = (seq_t*)malloc(seq_cap * sizeof(seq_t));
        seq_len = 0;
        cur_val = 0;
        seq_initialized = true;
    }

    while (seq_len < (size_t)n) {
        if (is_valid_number(cur_val)) {
            if (seq_len == seq_cap) {
                seq_cap <<= 1;
                seq = (seq_t*)realloc(seq, seq_cap * sizeof(seq_t));
            }
            seq[seq_len] = (seq_t)cur_val;
            ++seq_len;
        }
        ++cur_val;
    }
    return seq[n - 1];
}
