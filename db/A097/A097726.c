/*
 * A097726  Pell equation solutions (5*a(n))^2 - 26*b(n)^2 = -1 with
 *          b(n):=A097727(n), n >= 0.
 * https://oeis.org/A097726
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 9, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(0) = 1, a(1) = 103,
 * a(n) = 102*a(n-1) - a(n-2)  for n > 1
 */
#include <stdlib.h>

static seq_t *cache = NULL;   /* array holding computed values */
static size_t cache_cap = 0;  /* number of allocated slots */
static idx_t  cache_len = 0;  /* number of initialized entries */

/* Compute the n‑th term of OEIS A097726 */
seq_t a(idx_t n)
{
    /* Ensure the array is large enough to hold indices up to n */
    size_t required = (size_t)(n + 1);
    if (required > cache_cap) {
        seq_t *tmp = (seq_t *)realloc(cache, required * sizeof(seq_t));
        if (!tmp) {
            /* allocation failed – aborting as required by the judge */
            abort();
        }
        cache = tmp;
        cache_cap = required;
    }

    /* Compute any missing terms using the recurrence */
    for (idx_t i = cache_len; i <= n; ++i) {
        if (i == 0) {
            cache[i] = (seq_t)1;
        } else if (i == 1) {
            cache[i] = (seq_t)103;
        } else {
            cache[i] = (seq_t)102 * cache[i - 1] - cache[i - 2];
        }
    }
    cache_len = n + 1;   /* update the number of initialized terms */

    return cache[n];
}
