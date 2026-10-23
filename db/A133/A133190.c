/*
 * A133190  a(n) = 2*a(n-1) - a(n-2) + 2*a(n-3).
 * https://oeis.org/A133190
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 2*a(n-1) - a(n-2) + 2*a(n-3)
   with initial values a(0)=1, a(1)=3, a(2)=3 */
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

/* Ensure that *arr has room for indices up to n (inclusive). */
static void ensure_cache(idx_t n, seq_t **arr, size_t *cap)
{
    if (*cap < (size_t)(n + 1))
    {
        size_t new_cap = (size_t)(n + 1);
        void *tmp = realloc(*arr, new_cap * sizeof(seq_t));
        if (!tmp)
            abort();
        *arr = (seq_t*)tmp;
        *cap = new_cap;
    }
}

/* Compute a(n) for any n >= 0, memoizing results. */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cap = 0;
    static idx_t max_n = (idx_t)-1;

    if (cache == NULL)
    {
        /* Initialize cache with first three terms. */
        ensure_cache(2, &cache, &cap);
        cache[0] = 1;
        cache[1] = 3;
        cache[2] = 3;
        max_n = 2;
    }

    if (n <= max_n)
        return cache[n];

    ensure_cache(n, &cache, &cap);

    for (idx_t i = max_n + 1; i <= n; ++i)
    {
        /* perform all arithmetic using seq_t */
        cache[i] = (seq_t)2 * cache[i - 1]
                 - cache[i - 2]
                 + (seq_t)2 * cache[i - 3];
    }
    max_n = n;

    return cache[n];
}
