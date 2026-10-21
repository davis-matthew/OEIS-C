/*
 * A051806  Numbers with nonzero multiplicative digital root 4.
 * https://oeis.org/A051806
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   not stated
 * Range:     u8 n <= 35, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   root_is_four
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A051806: Numbers with non‑zero multiplicative digital root 4.
   A number whose digits d₁…dₖ (ignoring zeros) is repeatedly multiplied
   until a single digit is obtained.  The sequence consists of those
   numbers whose final digit is 4.  The function searches upward from 1,
   keeping a cache of found terms to answer arbitrary queries efficiently. */

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/root_is_four.h"

/* --- Cache of the sequence terms ------------------------------------ */

static seq_t *cache        = NULL;  /* array of found terms */
static size_t cache_len    = 0;     /* number of terms stored      */
static size_t cache_cap    = 0;     /* allocated array capacity   */
static seq_t next_candidate = 1;    /* next integer to test      */

/* Ensure that at least 'need' terms are in the cache. */
static void ensure_cache(idx_t need)
{
    while (cache_len < (size_t)need) {

        /* Grow the cache if necessary. */
        if (cache_len >= cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 64;
            seq_t *new_arr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!new_arr) { /* allocation failed – abort */
                exit(1);
            }
            cache = new_arr;
            cache_cap = new_cap;
        }

        /* Find the next integer that has multiplicative root 4. */
        while (true) {
            seq_t val = (seq_t)next_candidate;
            if (root_is_four(val)) {
                cache[cache_len++] = val;   /* store the term */
                ++next_candidate;          /* advance for the next search */
                break;
            }
            ++next_candidate;
        }
    }
}

/* Public sequence function. */
seq_t a(idx_t n)
{
    if (n == 0)            /* the sequence is defined for n ≥ 1 */
        return 0;
    ensure_cache(n);        /* guarantee at least n terms exist */
    return cache[n - 1];    /* cache is 0‑based, OEIS uses 1‑based */
}
