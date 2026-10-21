/*
 * A234790  Number of (n+1) X (2+1) 0..3 arrays with each 2 X 2 subblock
 *          having the number of clockwise edge increases less than or equal
 *          to the number of counterclockwise edge increases.
 * https://oeis.org/A234790
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 47*a(n-1) - 485*a(n-2) + 1429*a(n-3) - 1056*a(n-4) +
 *            208*a(n-5) - 40*a(n-6), with offset 1 and initial values
 *            a(1)=2504, a(2)=88072, a(3)=3015344, a(4)=102517088,
 *            a(5)=3479085232, a(6)=118012659040
 * Range:     u8 none, u16 n <= 1, u32 n <= 5, u64 n <= 11, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/* Cached array for memoization: a[0] unused. */
static seq_t *cache = NULL;
static size_t cap = 0;          /* allocated capacity */
static idx_t max_computed = 0;  /* highest index computed (0 if none) */

/* Initial values (index 1..6). */
static const seq_t init_vals[7] = {
    0,                                 /* a[0] unused */
    2504ULL,                           /* a[1] */
    88072ULL,                          /* a[2] */
    3015344ULL,                        /* a[3] */
    102517088ULL,                      /* a[4] */
    3479085232ULL,                     /* a[5] */
    118012659040ULL                    /* a[6] */
};

/* Ensure that cache contains values up to index n (1‑based). */
static void ensure_cache(idx_t n)
{
    /* Need array size at least n+1 (index 0 unused). */
    size_t need = (size_t)(n + 1);

    if (cache == NULL) {
        /* First allocation. */
        cache = (seq_t *)malloc(need * sizeof(seq_t));
        if (cache == NULL) exit(EXIT_FAILURE);
        cap = need;
        /* Populate initial entries. */
        cache[0] = (seq_t)0;
        for (idx_t i = 1; i <= 6; ++i) {
            cache[i] = init_vals[i];
        }
        max_computed = 6; /* all initial values computed */
    } else if (need > cap) {
        /* Grow the array. */
        seq_t *tmp = (seq_t *)realloc(cache, need * sizeof(seq_t));
        if (tmp == NULL) exit(EXIT_FAILURE);
        cache = tmp;
        cap = need;
    }

    if (n <= max_computed)
        return; /* already computed */

    /* Compute missing terms using the recurrence. */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        seq_t term = 0;
        term += (seq_t)47   * cache[i - 1];
        term -= (seq_t)485  * cache[i - 2];
        term += (seq_t)1429 * cache[i - 3];
        term -= (seq_t)1056 * cache[i - 4];
        term += (seq_t)208  * cache[i - 5];
        term -= (seq_t)40   * cache[i - 6];
        cache[i] = term;
    }
    max_computed = n;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0; /* safety fallback */
    if (n <= 6) {
        /* Return from constant table; no allocation needed. */
        return init_vals[n];
    }
    ensure_cache(n);
    return cache[n];
}
