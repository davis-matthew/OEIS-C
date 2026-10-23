/*
 * A129836  Nonnegative values x of solutions (x, y) to the Diophantine
 *          equation x^2 + (x + 97)^2 = y^2.
 * https://oeis.org/A129836
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-3) - a(n-6) + 194 (n > 6) with a(1)=0, a(2)=15,
 *            a(3)=228, a(4)=291, a(5)=368, a(6)=1575
 * Range:     u8 n <= 3, u16 n <= 12, u32 n <= 32, u64 n <= 68, u128 n <= 145,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>     /* malloc, calloc, free */

static seq_t *cache    = NULL;   /* index 0 unused, cache[1] holds a(1) */
static idx_t  cache_size = 0;    /* number of allocated slots (>= computed+1) */
static idx_t  computed   = 0;    /* largest index for which cache[i] is valid */

/* Ensure stored cache is large enough to hold indices up to n (inclusive) */
static void ensure_cache_size(idx_t n)
{
    if (cache_size > n)               /* already sufficient */
        return;

    idx_t new_size = cache_size ? cache_size * 2 : 8;
    while (new_size <= n)
        new_size *= 2;                 /* grow exponentially for speed */

    seq_t *new_cache = (seq_t *)calloc((size_t)new_size, sizeof(seq_t));
    if (!new_cache)                    /* allocation failed */
        return;                        /* keep old cache; caller will fail gracefully */

    /* copy already computed values */
    for (idx_t i = 1; i <= computed; ++i)
        new_cache[i] = cache[i];

    /* release old cache if it existed */
    free(cache);
    cache = new_cache;
    cache_size = new_size;
}

/* Compute the sequence up to index n (inclusive) and store results in cache */
static void compute_to(idx_t n)
{
    if (n <= computed)                 /* nothing to do */
        return;

    ensure_cache_size(n);

    for (idx_t i = computed + 1; i <= n; ++i) {
        if (i == 1)
            cache[i] = (seq_t)0;
        else if (i == 2)
            cache[i] = (seq_t)15;
        else if (i == 3)
            cache[i] = (seq_t)228;
        else if (i == 4)
            cache[i] = (seq_t)291;
        else if (i == 5)
            cache[i] = (seq_t)368;
        else if (i == 6)
            cache[i] = (seq_t)1575;
        else
            cache[i] = 6 * cache[i-3] - cache[i-6] + (seq_t)194;
    }
    computed = n;
}

seq_t a(idx_t n)
{
    if (n < 1)          /* undefined for n < 1 */
        return (seq_t)0;
    compute_to(n);
    return cache[n];
}
