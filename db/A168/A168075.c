/*
 * A168075  Expansion of (1+27x^2-54x^3)/((1+3x)^2*(1-3x+9 x^2)).
 * https://oeis.org/A168075
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   not stated
 * Range:     i8 n <= 2, i16 n <= 7, i32 n <= 16, i64 n <= 35, i128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Formula for A168075:
 *   a(n) = -3·a(n‑1) – 27·a(n‑3) – 81·a(n‑4)   for n ≥ 4
 *   with a(0)=1, a(1)=‑3, a(2)=36, a(3)=‑189
 * (This recurrence follows from the generating function
 *  (1+27x²‑54x³)/((1+3x)²·(1‑3x+9x²)).) */

#include <stddef.h>
#include <stdlib.h>

/* static cache for memoisation ---------------------------------------- */
static seq_t *cache = NULL;      /* array of computed terms             */
static size_t cache_cap = 0;     /* current capacity of cache array     */
static idx_t  cache_len = 0;     /* number of terms already stored      */

/* enlarge the cache if necessary and compute missing values             */
static void
ensure_cache_size(idx_t n)
{
    /* Allocate or grow the array to hold at least n+1 terms             */
    if (cache == NULL) {
        cache_cap = 1;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    }
    if (n + 1 > cache_cap) {
        size_t new_cap = cache_cap;
        while (new_cap < (size_t)(n + 1))
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) { /* allocation failure: abort the program */
            abort();
        }
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute terms that are not yet known                            */
    for (idx_t i = cache_len; i <= n; ++i) {
        if (i == 0) {
            cache[0] = (seq_t)1;
        } else if (i == 1) {
            cache[1] = (seq_t)-3;
        } else if (i == 2) {
            cache[2] = (seq_t)36;
        } else if (i == 3) {
            cache[3] = (seq_t)-189;
        } else {
            cache[i] = -(seq_t)3 * cache[i-1]
                     - (seq_t)27 * cache[i-3]
                     - (seq_t)81 * cache[i-4];
        }
    }
    cache_len = n + 1;
}

/* -------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    ensure_cache_size(n);
    return cache[n];
}
