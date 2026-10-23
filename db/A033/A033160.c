/*
 * A033160  Begins with (2, 4); avoids 3-term arithmetic progressions.
 * https://oeis.org/A033160
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the nth element of the greedy set of positive integers *
 *            containing 2 and 4 that avoids any 3‑term arithmetic progression
 * Range:     u8 n <= 32, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

/* ------------------------------------------------------------------ */
/*  Helper: dynamic array for the chosen sequence                     */
/* ------------------------------------------------------------------ */
static seq_t *chosen = NULL;
static size_t chosen_len = 0;
static size_t chosen_cap = 0;

/* Grow chosen array to hold at least 'need' elements */
static void grow_chosen(size_t need)
{
    if (need <= chosen_cap) return;
    if (chosen_cap == 0) chosen_cap = 8;
    while (chosen_cap < need) chosen_cap <<= 1;
    chosen = (seq_t *)realloc(chosen, chosen_cap * sizeof(seq_t));
}

/* ------------------------------------------------------------------ */
/*  Helper: dynamic bitset for membership testing                    */
/* ------------------------------------------------------------------ */
static uint8_t *present = NULL;   /* one byte per value (1 = present) */
static size_t present_size = 0;

/* Ensure we have room for index 'idx' (inclusive). */
static void ensure_present_size(size_t idx)
{
    if (idx < present_size) return;
    size_t newsize = present_size ? present_size : 8;
    while (newsize <= idx) newsize <<= 1;
    present = (uint8_t *)realloc(present, newsize);
    if (newsize > present_size) {
        memset(present + present_size, 0, newsize - present_size);
    }
    present_size = newsize;
}

/* ------------------------------------------------------------------ */
/*  Main function                                                    */
/* ------------------------------------------------------------------ */
static idx_t next_candidate = 3;   /* next integer to test */
static bool initialized = false;

/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* offset 1; values for n < 1 are unused */

    if (!initialized) {
        /* Pre‑populate with the first two numbers 2, 4 */
        grow_chosen(2);
        chosen[0] = (seq_t)2;
        chosen[1] = (seq_t)4;
        chosen_len = 2;

        /* Initialise present bitset up to value 4 */
        ensure_present_size(4);
        present[(size_t)2] = 1;
        present[(size_t)4] = 1;

        next_candidate = 3;
        initialized = true;
    }

    /* Build the sequence until we have at least n elements */
    while (chosen_len < (size_t)n) {
        seq_t cand = (seq_t)next_candidate;   /* candidate value */
        size_t idx = (size_t)next_candidate;

        /* Ensure present array can index 'cand' */
        ensure_present_size(idx);

        bool ok = true;

        /* 1. Check if cand would be the largest element of any AP */
        for (size_t j = 0; j < chosen_len; ++j) {
            seq_t b = chosen[j];
            if ((seq_t)2 * b > cand) {
                seq_t a = (seq_t)2 * b - cand;
                if (a > 0 && (size_t)a < present_size && present[(size_t)a]) {
                    ok = false;
                    break;
                }
            }
        }

        /* 2. Check if cand would be the middle element of any AP */
        if (ok) {
            size_t l = 0;
            size_t r = chosen_len - 1;
            seq_t target = (seq_t)2 * cand;
            while (l < r) {
                seq_t sum = chosen[l] + chosen[r];
                if (sum == target) {
                    ok = false;
                    break;
                } else if (sum < target) {
                    ++l;
                } else {
                    --r;
                }
            }
        }

        /* If candidate is valid, add it to the sequence */
        if (ok) {
            grow_chosen(chosen_len + 1);
            chosen[chosen_len++] = cand;
            present[idx] = 1;
        }

        ++next_candidate;
    }

    return chosen[n - 1];
}
