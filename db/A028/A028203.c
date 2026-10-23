/*
 * A028203  Expansion of 1/((1-6x)(1-7x)(1-8x)(1-12x)).
 * https://oeis.org/A028203
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A028203: Expansion of 1/((1-6x)(1-7x)(1-8x)(1-12x))
   Formula (recurrence): a(0)=1, a(1)=33
   a(n) = 20*a(n‑1) - 96*a(n‑2) + 7^(n+1) - 6^(n+1),   n >= 2
*/

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

static seq_t *cache = NULL;          /* cached sequence values      */
static size_t cache_cap = 0;         /* allocated capacity          */
static size_t cache_len = 0;         /* number of values computed   */
static seq_t pow6 = (seq_t)216;      /* 6^(i+1) for current i (i>=2) */
static seq_t pow7 = (seq_t)343;      /* 7^(i+1) for current i (i>=2) */

/* Ensure values are computed up to index n (inclusive) */
static void ensure_cache(idx_t n)
{
    /* Allocate initial storage if needed */
    if (cache_len == 0) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) { /* allocation failure – nothing more we can do */ return; }
        cache[0] = (seq_t)1;   /* a(0) */
        cache[1] = (seq_t)33;  /* a(1) */
        cache_len = 2;
        /* pow6 and pow7 are already 6^3 and 7^3 for i = 2 */
    }

    /* Grow storage if necessary */
    while (cache_len <= (size_t)n) {
        if (cache_len == cache_cap) {
            size_t new_cap = cache_cap * 2;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) { /* allocation failure – abort further growth */ break; }
            cache = tmp;
            cache_cap = new_cap;
        }

        /* Compute a(cache_len) using the recurrence */
        seq_t term;
        if (cache_len == 0) {
            term = (seq_t)1;
        } else if (cache_len == 1) {
            term = (seq_t)33;
        } else {
            /* a(n) = 20*a(n-1) - 96*a(n-2) + 7^(n+1) - 6^(n+1) */
            term = cache[cache_len - 1] * (seq_t)20
                 - cache[cache_len - 2] * (seq_t)96
                 + pow7
                 - pow6;
        }

        cache[cache_len] = term;
        cache_len++;

        /* Advance the powers for the next index */
        pow6 *= (seq_t)6;   /* next 6^(i+1) */
        pow7 *= (seq_t)7;   /* next 7^(i+1) */
    }
}

/* Sequence entry point */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[(size_t)n];
}
