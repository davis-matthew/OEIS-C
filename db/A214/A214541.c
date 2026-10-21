/*
 * A214541  Number of 3X3X3 triangular 0..n arrays with no element lying
 *          outside the (possibly reversed) range delimited by its sw and se
 *          neighbors, and every horizontal row having the same average value.
 * https://oeis.org/A214541
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 29, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A214541 – Number of 3X3X3 triangular 0..n arrays with no element lying outside the (possibly reversed) range delimited by its sw and se neighbors, and every horizontal row having the same average value.
   Recurrence (empirical): a(n) = 2*a(n-1) - 3*a(n-4) + 3*a(n-6) - 2*a(n-9) + a(n-10),
   with initial values
   a(1)=2, a(2)=15, a(3)=38, a(4)=79, a(5)=152, a(6)=263,
   a(7)=418, a(8)=643, a(9)=942, a(10)=1329.
   The function below memoises results and works for any n ≥ 1. */

#include <stdlib.h>   /* malloc, realloc */
#include <stddef.h>   /* size_t */

static void ensure_cache(idx_t need, seq_t **p, size_t *cap)
{
    /* enlarge cache to hold indices 0..need */
    if (*cap <= (size_t)need) {
        size_t new_cap = (size_t)need + 1;
        if (*p == NULL)
            *p = (seq_t *)malloc(new_cap * sizeof(seq_t));
        else
            *p = (seq_t *)realloc(*p, new_cap * sizeof(seq_t));
        if (*p == NULL) {
            /* allocation failed – terminate the program */
            abort();
        }
        *cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;   /* allocated size */
    static idx_t max_done = 0;    /* highest n for which cache[n] is valid */

    if (n == 0)   /* undefined for n=0 – return 0 as sentinel */
        return 0;

    /* quick return if already computed */
    if (n <= max_done)
        return cache[n];

    /* ensure the cache can hold up to index n */
    ensure_cache(n, &cache, &cache_cap);

    /* initialise the first 10 terms if not already done */
    if (max_done < 10) {
        static const seq_t init_vals[11] = {
            0, /* dummy for index 0 */
            2, 15, 38, 79, 152, 263, 418, 643, 942, 1329
        };

        idx_t start = (max_done < 1) ? 1 : (idx_t)(max_done + 1);
        idx_t init_len = (n < 10) ? n : 10;
        for (idx_t i = start; i <= init_len; ++i) {
            cache[i] = init_vals[i];
        }
        max_done = init_len;
        if (n <= max_done)
            return cache[n];
    }

    /* compute remaining terms using the recurrence */
    for (idx_t i = max_done + 1; i <= n; ++i) {
        seq_t term = cache[i - 10];
        term += (seq_t)2 * cache[i - 1];
        term -= (seq_t)3 * cache[i - 4];
        term += (seq_t)3 * cache[i - 6];
        term -= (seq_t)2 * cache[i - 9];
        cache[i] = term;
    }

    max_done = n;
    return cache[n];
}
