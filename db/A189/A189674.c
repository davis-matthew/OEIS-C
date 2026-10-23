/*
 * A189674  Partial sums of A189673.
 * https://oeis.org/A189674
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(floor((n+2)/3)) + floor((n+1)/3), a(1)=0
 * Range:     u8 n <= 81, u16 n <= 81, u32 n <= 81, u64 n <= 81, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS A189674 – partial sums of A189673 */

#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>

static seq_t   *cache = NULL;   /* 1‑based storage; cache[0] unused */
static size_t   cap    = 0;      /* allocated size of cache  (in elements) */
static idx_t    max_computed = 0;/* largest n for which a(n) has been set */

/* Ensure the cache can accommodate indices up to n (inclusive) */
static void
ensure_capacity(idx_t n)
{
    if ((size_t)n + 1 <= cap)          /* already enough space */
        return;

    size_t new_cap = (cap ? cap * 2 : 16);
    while (new_cap <= (size_t)n)
        new_cap <<= 1;                 /* grow by factor 2 until large enough */

    /* Reallocate memory, casting to the correct pointer type */
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();                      /* allocation failed */

    cache = tmp;
    cap   = new_cap;
    /* Avoid initializing all elements; they will be set on demand */
}

/* Recursive definition implementation with memoisation */
seq_t a(idx_t n)
{
    if (n < 1)                /* undefined for n < 1, return 0 */
        return (seq_t)0;

    ensure_capacity(n);

    /* Initialise base case once */
    if (max_computed == 0) {
        cache[1] = (seq_t)0;
        max_computed = 1;
    }

    /* Compute all missing terms up to n */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        idx_t  m    = (i + 2) / 3;                /* floor((i+2)/3) */
        seq_t  term = (seq_t)((i + 1) / 3);       /* floor((i+1)/3) */
        cache[i] = cache[m] + term;
    }

    max_computed = n;
    return cache[n];
}
