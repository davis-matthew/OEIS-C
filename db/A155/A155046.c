/*
 * A155046  List of pairs: first pair is (1,1); then follow (x,y) with (x+2y,
 *          x+y).
 * https://oeis.org/A155046
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabf
 * Formula:   a(n) = 2*a(n-2) + a(n-4) for n > 4, with a(1)=1, a(2)=1, a(3)=3,
 *            a(4)=2. This generates the OEIS sequence A155046
 * Range:     u8 n <= 14, u16 n <= 26, u32 n <= 50, u64 n <= 102,
 *            u128 n <= 202, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/* Static cache for sequence values */
static seq_t *cache = NULL;
static idx_t max_cached = 0;

/* Ensure that cache holds terms up to and including n */
static void ensure_cache(idx_t n)
{
    if (n <= max_cached)
        return;

    /* Allocate new storage */
    seq_t *new_cache;
#ifdef __cplusplus
    new_cache = new seq_t[(size_t)(n + 1)];
#else
    new_cache = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
    if (!new_cache)
        abort();
#endif

    /* Copy old values */
    for (idx_t i = 1; i <= max_cached; ++i)
        new_cache[i] = cache[i];

    /* Set base terms if not already present */
    if (n >= 1)
        new_cache[1] = (seq_t)1;
    if (n >= 2)
        new_cache[2] = (seq_t)1;
    if (n >= 3)
        new_cache[3] = (seq_t)3;
    if (n >= 4)
        new_cache[4] = (seq_t)2;

    /* Compute missing values using the recurrence */
    for (idx_t i = max_cached + 1; i <= n; ++i) {
        if (i <= 4)     /* base terms already handled */
            continue;
        new_cache[i] = (seq_t)2 * new_cache[i - 2] + new_cache[i - 4];
    }

    /* Release old cache */
#ifdef __cplusplus
    delete[] cache;
#else
    free(cache);
#endif

    cache     = new_cache;
    max_cached = n;
}

/* Public function required by the build system */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;           /* sequence only defined for n≥1 */

    ensure_cache(n);
    return cache[n];
}
