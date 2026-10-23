/*
 * A027980  a(n) = Sum_{k=0..n-1} T(n,k)*T(n,2n-k), T given by A027960.
 * https://oeis.org/A027980
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n+1)*Lucas(2*n) - Fibonacci(2*n+1) - (-1)^n (implemented
 *            via the linear recurrence) a(0)=1, a(1)=13, a(2)=48, a(3)=176,
 *            a(4)=580 a(n) = 5*a(n-1) - 5*a(n-2) - 5*a(n-3) + 5*a(n-4) -
 *            a(n-5) (n>=5)
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 18, u64 n <= 41, u128 n <= 86,
 *            big any n
 * Helpers:   seq_alloc_new_array, seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/seq_alloc_new_array.h"
#include "../utils/seq_free_array.h"

/* static cache for memoisation */
static seq_t *cache = NULL;       /* array of computed terms */
static idx_t cache_len = 0;       /* number of elements stored (max index+1) */

/* Ensure that cache holds terms up to index n */
static void ensure_cache(idx_t n)
{
    if (n < cache_len)
        return;                   /* already cached */

    /* Allocate a new array of the required size */
    seq_t *new_cache = seq_alloc_new_array(n + 1);
    if (!new_cache) {
        /* allocation failed */
        exit(EXIT_FAILURE);
    }

    /* Copy existing values to the new array */
    for (idx_t i = 0; i < cache_len; ++i)
        new_cache[i] = cache[i];

    /* Free the old array (works for arbitrary‑precision types) */
    if (cache)
        seq_free_array(cache);

    cache = new_cache;
    cache_len = n + 1;

    /* Base values for n = 0..4 */
    if (cache_len >= 1) cache[0] = (seq_t)1;
    if (cache_len >= 2) cache[1] = (seq_t)13;
    if (cache_len >= 3) cache[2] = (seq_t)48;
    if (cache_len >= 4) cache[3] = (seq_t)176;
    if (cache_len >= 5) cache[4] = (seq_t)580;

    /* Compute missing terms using the recurrence */
    for (idx_t i = 5; i <= n; ++i) {
        seq_t term = cache[i - 1] * (seq_t)5;
        term -= cache[i - 2] * (seq_t)5;
        term -= cache[i - 3] * (seq_t)5;
        term += cache[i - 4] * (seq_t)5;
        term -= cache[i - 5];
        cache[i] = term;
    }
}

/* Public function required by the specification */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
