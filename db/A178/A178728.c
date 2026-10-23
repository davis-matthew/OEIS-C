/*
 * A178728  Number of terms in a "no English letters in common" sequence.
 * https://oeis.org/A178728
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, word
 * Formula:   not stated
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   word_to_mask
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* O(E) brute‑force search using English‑letter masks. */
/* For each n, start with n and repeatedly append the smallest
   unused integer whose English name has no letters in common
   with the English name of the previous term.  Return the
   number of terms produced (including the start). */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/word_to_mask.h"

/* word tables – used to build the masks */
static const char *const UNIT_WORDS[10] = {
    "zero","one","two","three","four","five","six","seven","eight","nine"
};
static const char *const TEEN_WORDS[10] = {
    "ten","eleven","twelve","thirteen","fourteen","fifteen",
    "sixteen","seventeen","eighteen","nineteen"
};
static const char *const TENS_WORDS[10] = {
    "","", "twenty","thirty","forty","fifty",
    "sixty","seventy","eighty","ninety"
};
static const char *const SCALE_WORDS[20] = {
    "","thousand","million","billion","trillion","quadrillion",
    "quintillion","sextillion","septillion","octillion",
    "nonillion","decillion","undecillion","duodecillion",
    "tredecillion","quattuordecillion","quindecillion",
    "sexdecillion","septendecillion","octodecillion"
};
static const char *const HUNDRED_WORD = "hundred";

/* Pre‑computed masks for the above words */
static uint32_t UNIT_MASKS[10];
static uint32_t TEEN_MASKS[10];
static uint32_t TENS_MASKS[10];
static uint32_t SCALE_MASKS[20];
static uint32_t HUNDRED_MASK;
static uint32_t ZERO_MASK;
static uint32_t UNIVERSAL_MASK;
static bool MASKS_INIT = false;

/* Initialise all masks once */
static inline void init_masks(void)
{
    if (MASKS_INIT) return;
    for (int i = 0; i < 10; ++i) UNIT_MASKS[i] = word_to_mask(UNIT_WORDS[i]);
    for (int i = 0; i < 10; ++i) TEEN_MASKS[i] = word_to_mask(TEEN_WORDS[i]);
    for (int i = 0; i < 10; ++i) TENS_MASKS[i] = word_to_mask(TENS_WORDS[i]);
    for (int i = 0; i < 20; ++i) SCALE_MASKS[i] = word_to_mask(SCALE_WORDS[i]);
    HUNDRED_MASK = word_to_mask(HUNDRED_WORD);
    ZERO_MASK    = UNIT_MASKS[0];

    UNIVERSAL_MASK = 0;
    for (int i = 0; i < 10; ++i) UNIVERSAL_MASK |= UNIT_MASKS[i];
    for (int i = 0; i < 10; ++i) UNIVERSAL_MASK |= TEEN_MASKS[i];
    for (int i = 0; i < 10; ++i) UNIVERSAL_MASK |= TENS_MASKS[i];
    for (int i = 0; i < 20; ++i) UNIVERSAL_MASK |= SCALE_MASKS[i];
    UNIVERSAL_MASK |= HUNDRED_MASK;
    MASKS_INIT = true;
}

/* Compute letter mask of the English representation of n */
static inline uint32_t mask_of(idx_t n)
{
    if (n == 0) return ZERO_MASK;
    uint32_t m = 0;
    idx_t tmp = n;
    int group = 0;
    while (tmp != 0) {
        int part = tmp % 1000;
        tmp /= 1000;
        if (part != 0) {
            if (part >= 100) {
                m |= UNIT_MASKS[part / 100];
                m |= HUNDRED_MASK;
            }
            int rem = part % 100;
            if (rem >= 20) {
                m |= TENS_MASKS[rem / 10];
                if (rem % 10) m |= UNIT_MASKS[rem % 10];
            } else if (rem >= 10) {
                m |= TEEN_MASKS[rem - 10];
            } else if (rem > 0) {
                m |= UNIT_MASKS[rem];
            }
            if (group > 0) m |= SCALE_MASKS[group];
        }
        ++group;
    }
    return m;
}

/* Ensure used array can hold index idx */
static inline void ensure_used_capacity(uint8_t **used, size_t *cap, idx_t idx)
{
    if ((size_t)idx < *cap) return;
    size_t newcap = (*cap > 0) ? *cap : 128;
    while ((size_t)idx >= newcap) newcap <<= 1;
    uint8_t *newptr = (uint8_t *)realloc(*used, newcap);
    if (!newptr) exit(1);
    memset(newptr + *cap, 0, newcap - *cap);
    *used = newptr;
    *cap = newcap;
}

/* The required OEIS function */
seq_t a(idx_t n)
{
    init_masks();

    size_t used_cap = 128;
    uint8_t *used = (uint8_t *)calloc(used_cap, 1);
    if (!used) exit(1);

    /* Mark the starting term as used */
    ensure_used_capacity(&used, &used_cap, n);
    used[n] = 1;

    idx_t curr = n;
    seq_t len = 1;                    /* include start */
    uint32_t prev_mask = mask_of(curr);

    /* A reasonable upper bound for candidate search – safe for n <= 10000 */
    const idx_t MAX_CAND = 200000;

    while (1) {
        bool found = false;
        for (idx_t cand = 0; cand <= MAX_CAND; ++cand) {
            if (cand > MAX_CAND) break;
            ensure_used_capacity(&used, &used_cap, cand);
            if (!used[cand]) {
                uint32_t cand_mask = mask_of(cand);
                if ((cand_mask & prev_mask) == 0) {
                    used[cand] = 1;
                    curr = cand;
                    prev_mask = cand_mask;
                    ++len;
                    found = true;
                    break;
                }
            }
        }
        if (!found) break; /* no candidate found within bound – terminate */
    }

    free(used);
    return len;
}
