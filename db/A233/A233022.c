/*
 * A233022  Number of nX4 0..3 arrays with no element x(i,j) adjacent to value
 *          3-x(i,j) horizontally, vertically, diagonally or antidiagonally,
 *          and top left element zero.
 * https://oeis.org/A233022
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A233022: Number of n×4 arrays with elements 0..3 such that no cell
   contains 3‑x adjacent horizontally, vertically, diagonally or antidiagonally.
   Recurrence (order 8):                                   */
/* a(n) =  23*a(n-1) - 39*a(n-2) - 265*a(n-3) + 180*a(n-4)
 *         + 655*a(n-5) - 156*a(n-6) - 127*a(n-7) + 20*a(n-8)   */
/* Initial values: a(1)=27, a(2)=435, a(3)=8835, a(4)=179999,
 *                 a(5)=3685017, a(6)=75489671, a(7)=1546718177,
 *                 a(8)=31692006853 */
#include <stdlib.h>
#include <stdbool.h>

extern void ensure_cache(idx_t n, seq_t **cache_ptr, size_t *cap_ptr);

static seq_t *cache = NULL;   /* 0‑based array of computed terms */
static idx_t cached = 0;      /* number of terms currently stored in cache */

/* Base values for n = 1 … 8 (0‑based indices 0 … 7) */
static const seq_t base_vals[8] = {
    (seq_t)27,
    (seq_t)435,
    (seq_t)8835,
    (seq_t)179999,
    (seq_t)3685017,
    (seq_t)75489671,
    (seq_t)1546718177,
    (seq_t)31692006853
};

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* undefined for offset <1 */

    /* If sequence not yet initialized, allocate enough space for n or at
       least 8 terms (so that we can use base values). */
    if (cached == 0) {
        size_t init_cap = (n > 8) ? (size_t)n : 8;
        cache = (seq_t *)realloc(cache, init_cap * sizeof(seq_t));
        for (size_t i = 0; i < 8; ++i) cache[i] = base_vals[i];
        cached = 8;  /* we have computed terms a(1) … a(8) */
    }

    /* If requested term already computed, return it. */
    if (n <= cached) return cache[n - 1];

    /* Extend cache if necessary. */
    if (cached < n) {
        size_t new_cap = (size_t)n;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (new_cache == NULL) {
            /* Allocation failure; in a real system you might abort or
               return an error value. Here we simply keep the old cache. */
            return (seq_t)0;
        }
        cache = new_cache;
        /* Compute new terms using the recurrence. */
        for (idx_t k = cached; k < n; ++k) {  /* k is zero‑based index of a(k+1) */
            cache[k] = ( (seq_t)23 * cache[k-1]
                       - (seq_t)39 * cache[k-2]
                       - (seq_t)265 * cache[k-3]
                       + (seq_t)180 * cache[k-4]
                       + (seq_t)655 * cache[k-5]
                       - (seq_t)156 * cache[k-6]
                       - (seq_t)127 * cache[k-7]
                       + (seq_t)20  * cache[k-8] );
        }
        cached = n;
    }

    return cache[n - 1];
}
