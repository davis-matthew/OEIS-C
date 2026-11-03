/*
 * A049361  Digitally balanced numbers in both bases 2 and 3.
 * https://oeis.org/A049361
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 140, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 56 terms through n = 9802 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A049361: Digitally balanced numbers in both bases 2 and 3.
   The function returns the n‑th smallest integer whose binary
   representation has an equal number of 0‑bits and 1‑bits
   (including the leading 1) and whose ternary representation has
   an equal number of each of the digits 0, 1, 2.  The sequence is
   generated incrementally and cached for subsequent calls. */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Compute the next combination with the same number of set bits. */
static idx_t next_combination_mask(idx_t v)
{
    if (v == 0) return 0;
    idx_t lowest = v & -v;          /* lowest set bit               */
    idx_t next   = v + lowest;      /* add that bit, clears low 1s  */
    idx_t changes = v ^ next;       /* bits that changed           */
    changes = (changes >> 2) / lowest;
    return next | changes;
}

static bool balanced_ternary(seq_t x)
{
    unsigned int cnt[3] = {0, 0, 0};
    while (x > 0) {
        cnt[(int)(x % 3)]++;
        x /= 3;
    }
    return cnt[0] == cnt[1] && cnt[1] == cnt[2] && cnt[0] > 0;
}

seq_t a(idx_t n)
{
    /* static cache of already found terms */
    static seq_t *cache   = NULL;
    static size_t cache_len = 0, cache_cap = 0;

    /* state for generating binary‑balanced candidates */
    static bool initialized = false;
    static idx_t cur_L      = 2;   /* current even binary length (>=2)   */
    static idx_t cur_r      = 0;   /* number of ones among lower bits   */
    static idx_t cur_mask   = 0;   /* mask of lower bits                */

    if (!initialized) {
        cache_cap = 256;
        cache = mem_new(seq_t, cache_cap);
        initialized = true;
    }

    /* generate terms until we have at least n of them */
    while (cache_len < n) {
        /* candidate value */
        seq_t val = ((seq_t)1 << (cur_L - 1)) | (seq_t)cur_mask;

        if (balanced_ternary(val)) {
            if (cache_len == cache_cap) {
                size_t newcap = cache_cap ? cache_cap * 2 : 8;
                seq_t *tmp = mem_resize(seq_t, cache, newcap);
                if (!tmp) abort();           /* allocation failed */
                cache = tmp;
                cache_cap = newcap;
            }
            cache[cache_len++] = val;
        }

        /* advance to the next binary‑balanced number */
        if (cur_r == 0) {                       /* only mask 0 for L=2    */
            cur_L += 2;
            cur_r   = cur_L / 2 - 1;
            cur_mask = (cur_r > 0) ? ((idx_t)1 << cur_r) - 1 : 0;
            continue;
        }

        idx_t hiMask = (((idx_t)1 << cur_r) - 1) << ((cur_L - 1) - cur_r);
        idx_t nextMask = next_combination_mask(cur_mask);

        if (nextMask == 0 || nextMask > hiMask) {
            cur_L += 2;
            cur_r   = cur_L / 2 - 1;
            cur_mask = (cur_r > 0) ? (((idx_t)1 << cur_r) - 1) : 0;
        } else {
            cur_mask = nextMask;
        }
    }

    return cache[(size_t)(n - 1)];
}
