/*
 * A141533  The first subdiagonal of the array of A141425 and its higher order
 *          differences.
 * https://oeis.org/A141533
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = a(n-1) - a(n-2) + 3*a(n-3) + 6*a(n-4) (n >= 5) * Initial
 *            values: a(1) = 1, a(2) = -1, a(3) = -2, a(4) = 23
 * Range:     i8 n <= 7, i16 n <= 15, i32 n <= 31, i64 n <= 34, i128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

static seq_t *cache = NULL;
static size_t cache_cap = 0;   /* allocated capacity (in terms) */
static idx_t cache_size = 0;   /* number of terms already computed */

/* ensure cache has room for at least n terms (1‑based index) */
static void ensure_capacity(idx_t n) {
    if (cache_cap < (size_t)n) {
        size_t new_cap = cache_cap ? cache_cap : 4;
        while (new_cap < (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }
}

/* compute terms up to n (inclusive), using recurrence */
static void compute_up_to(idx_t n) {
    if (cache_size >= n) return;

    if (cache_size == 0) {
        /* initialise first terms */
        cache[0] = 1;                   /* a(1) */
        if (n > 1) cache[1] = -1;       /* a(2) */
        if (n > 2) cache[2] = -2;       /* a(3) */
        if (n > 3) cache[3] = 23;       /* a(4) */
        cache_size = (n > 4 ? 4 : n);
    }

    /* if we still lack any of the first four terms, fill them now */
    if (cache_size < 4 && n > 1) {
        if (cache_size == 1) {
            cache[1] = -1;
            cache_size = 2;
        }
    }
    if (cache_size < 4 && n > 2) {
        if (cache_size == 2) {
            cache[2] = -2;
            cache_size = 3;
        }
    }
    if (cache_size < 4 && n > 3) {
        if (cache_size == 3) {
            cache[3] = 23;
            cache_size = 4;
        }
    }

    /* extend the sequence */
    while (cache_size < n) {
        idx_t idx = cache_size;        /* zero‑based index for a(idx+1) */
        seq_t val = cache[idx-1] - cache[idx-2] + 3*cache[idx-3] + 6*cache[idx-4];
        cache[idx] = val;
        ++cache_size;
    }
}

seq_t a(idx_t n) {
    if (n == 0) return 0; /* undefined for n < 1, return 0 as safe fallback */
    ensure_capacity(n);
    compute_up_to(n);
    return cache[n-1];
}
