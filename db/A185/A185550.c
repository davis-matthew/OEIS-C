/*
 * A185550  Numbers not of the form ceiling(n^(3/2)); complement of A185549.
 * https://oeis.org/A185550
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 100, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   alloc_seq_array, floor_sqrt, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Numbers not of the form ⌈k^(3/2)⌉ (OEIS A185550).
   The function returns the n‑th such number (1‑based). */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/floor_sqrt.h"
#include "../utils/alloc_seq_array.h"
#include "../utils/seq_realloc.h"

/* Compute ceil(m^(3/2)) using integer arithmetic. */
static inline seq_t ceil_m(idx_t m)
{
    /* m^3 */
    seq_t t = (seq_t)m * (seq_t)m * (seq_t)m;
    seq_t sq = floor_sqrt(t);            /* ⌊√t⌋ */
    return (sq * sq == t) ? sq : sq + 1; /* ⌈√t⌉ */
}

/* Memoization state ----------------------------------------------------- */
static seq_t *cache   = NULL;   /* 1‑based array of computed terms        */
static size_t cache_cap = 0;    /* allocated capacity (>= cache_len+1)    */
static idx_t  cache_len = 0;    /* number of terms already computed      */

static seq_t last_x    = 0;     /* last integer examined                 */
static idx_t  last_m    = 1;    /* last m used for ceil_m                */
static seq_t  last_ceil = 0;    /* ceil_m(last_m)                        */

/* Ensure the cache contains at least n terms --------------------------- */
static void ensure_cache(idx_t n)
{
    if ((idx_t)n <= cache_len)
        return;

    /* Resize the cache if required. */
    if ((size_t)(n + 1) > cache_cap) {
        size_t new_cap = (size_t)(n + 1);
        seq_t *tmp = seq_realloc(cache, cache_cap, new_cap);
        if (!tmp) abort();           /* fatal error – cannot proceed */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Generate terms from (cache_len + 1) up to n. */
    seq_t x = last_x + 1;           /* the next integer to examine */

    if (cache_len == 0) {           /* first run – initialise ceil_m */
        last_m    = 1;
        last_ceil = ceil_m(1);
    }

    while (cache_len < (idx_t)n) {
        /* Advance the ceil_m sequence until it is >= x. */
        while (x > last_ceil) {
            ++last_m;
            last_ceil = ceil_m(last_m);
        }

        if (x == last_ceil) {       /* x is of the forbidden form – skip it */
            ;
        } else {
            cache[++cache_len] = x; /* store as a valid term */
        }
        ++x;
    }
    last_x = x - 1;                 /* last examined integer */
}

/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* Offset 1: a(1) is the first term (value 2). */
    if (n < 1) return 0;           /* undefined for n = 0 – simply return 0 */
    ensure_cache(n);
    return cache[n];
}
