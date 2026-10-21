/*
 * A076039  Start with 1. Multiply or divide by n accordingly as a(n-1) is
 *          smaller or greater than n and then take the integer value (this is
 *          to ensure that a(n) > 0 for all n).
 * https://oeis.org/A076039
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nice, nonn, look
 * Formula:   not stated
 * Range:     u8 n <= 41, u16 n <= 365, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=1, a(n) = n*a(n-1) if a(n-1)<n, otherwise floor(a(n-1)/n) */

#include <stdlib.h>
#include <stdbool.h>

static seq_t *cache = NULL;
static size_t cache_len = 0;  /* number of stored terms (a(1) at cache[0]) */
static size_t cache_cap = 0;  /* current allocated capacity */

/* enlarge cache until it can hold at least 'needed' elements */
static void ensure_capacity(size_t needed)
{
    if (needed <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    if (new_cap < needed)
        new_cap = needed;
    seq_t *new_ptr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = new_ptr;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0)           /* undefined for offset 1; return 0 as a safe default */
        return 0;

    /* make sure the cache can hold a(n) */
    ensure_capacity((size_t)n);

    /* initialise first value lazily */
    if (cache_len == 0) {
        cache[0] = (seq_t)1; /* a(1) = 1 */
        cache_len = 1;
    }

    /* compute missing terms up to a(n) */
    while (cache_len < n) {
        idx_t cur = (idx_t)cache_len + 1;           /* current index we are computing */
        seq_t prev = cache[cache_len - 1];          /* a(cur-1) */

        if (prev < (seq_t)cur)
            cache[cache_len] = prev * (seq_t)cur;   /* multiply */
        else
            cache[cache_len] = prev / (seq_t)cur;   /* floor division */

        cache_len++;
    }

    return cache[n - 1];
}
