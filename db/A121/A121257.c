/*
 * A121257  Number of conjugated cycles composed of six carbons in
 *          (1,1)-nanotubes in terms of the number of naphthalene units.
 * https://oeis.org/A121257
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*a(n-1) - 11*a(n-2) + 6*a(n-3) - a(n-4); Initial values:
 *            a(1)=4, a(2)=20, a(3)=76, a(4)=260
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 19, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/* static cache: 1‑based indexing, cache[0] is unused to avoid
   accessing a(n-4) when n <= 4. */
static seq_t *cache = NULL;
static idx_t  cache_size = 0;      /* highest index currently stored */

/* Ensure that cache holds values 1..n (and at least 1..4). */
static void ensure_cache(idx_t n)
{
    if (n == 0) return;               /* nothing to do for n=0 */

    /* Allocate or enlarge the cache as needed. */
    if (cache == NULL) {
        /* Allocate enough space for the largest of n and 4. */
        size_t need = (size_t)(n > 4 ? n : 4) + 1;   /* +1 to make 1‑based simple */
        cache = (seq_t*)malloc(need * sizeof(seq_t));
        if (cache == NULL) abort();
        cache_size = 0;
    } else if (n > cache_size) {
        size_t need = (size_t)(n + 1);
        cache = (seq_t*)realloc(cache, need * sizeof(seq_t));
        if (cache == NULL) abort();
    }

    /* Initialise the first four terms if we have not already. */
    if (cache_size < 4) {
        cache[1] = (seq_t)4;
        cache[2] = (seq_t)20;
        cache[3] = (seq_t)76;
        cache[4] = (seq_t)260;
        cache_size = 4;
    }

    /* Compute any remaining terms up to n. */
    for (idx_t i = cache_size + 1; i <= n; ++i) {
        seq_t a_nm1 = cache[(size_t)(i - 1)];
        seq_t a_nm2 = cache[(size_t)(i - 2)];
        seq_t a_nm3 = cache[(size_t)(i - 3)];
        seq_t a_nm4 = cache[(size_t)(i - 4)];

        seq_t term = (seq_t)6 * a_nm1;
        term = term - (seq_t)11 * a_nm2;
        term = term + (seq_t)6 * a_nm3;
        term = term - a_nm4;

        cache[(size_t)i] = term;
    }

    if (n > cache_size) cache_size = n;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* Undefined, but safe return for n=0 */
    ensure_cache(n);
    return cache[(size_t)n];
}
