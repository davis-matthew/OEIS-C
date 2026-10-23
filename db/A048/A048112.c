/*
 * A048112  a(1) = 1, a(2) = 1, a(3) = 1, a(n) = a(n-3) * (a(n-2) + a(n-1)).
 * https://oeis.org/A048112
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1) = 1, a(2) = 1, a(3) = 1, a(n) = a(n-3) * (a(n-2) + a(n-1))
 * Range:     u8 n <= 8, u16 n <= 10, u32 n <= 12, u64 n <= 14, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static seq_t *cache = 0;          /* 1‑based array of computed terms */
static size_t cap = 0;            /* current capacity of cache   */
static idx_t computed = 0;        /* highest n that has been computed */

/* ensure that cache has room for indices up to needed (inclusive) */
static void ensure_capacity(size_t needed)
{
    if (needed <= cap)
        return;

    size_t new_cap = (cap < 4) ? 4 : cap * 2;
    while (new_cap < needed)
        new_cap *= 2;

    seq_t *new_cache;
#ifdef __cplusplus
    new_cache = new seq_t[new_cap];
    /* copy existing values into the new array */
    for (size_t i = 1; i <= cap; ++i) {
        new_cache[i] = cache[i];
    }
    if (cache) delete[] cache;
#else
    new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (!new_cache) return; /* allocation failure – unlikely for n ~ 10000 */
    for (size_t i = 1; i <= cap; ++i) {
        new_cache[i] = cache[i];
    }
    if (cache) free(cache);
#endif

    cache = new_cache;
    cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* undefined for n=0, but return 0 safely */

    size_t required = (size_t)n + 1;   /* +1 because index 0 unused */
    ensure_capacity(required);

    if (n <= computed)
        return cache[n];

    for (idx_t i = computed + 1; i <= n; ++i) {
        if (i <= 3) {
            cache[i] = 1;
        } else {
            cache[i] = cache[i-3] * (cache[i-2] + cache[i-1]);
        }
    }
    computed = n;
    return cache[n];
}
