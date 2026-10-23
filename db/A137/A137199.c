/*
 * A137199  a(n)=a(n-1)+3a(n-2)+a(n-3).
 * https://oeis.org/A137199
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + 3*a(n-2) + a(n-3), with a(0) = a(1) = a(2) = 1
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 26, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;       /* memoization buffer */
static size_t cache_cap = 0;      /* allocated capacity (number of slots) */
static size_t cache_len = 0;      /* number of values already computed */

/* Ensure that the cache has at least ``need`` slots.  The capacity is
   grown to the larger of ``need`` and twice the current size, to
   keep amortised O(1) growth. */
static void ensure_capacity(size_t need)
{
    if (need <= cache_cap)
        return;
    size_t new_cap = (cache_cap == 0) ? 4 : cache_cap * 2;
    if (new_cap < need)
        new_cap = need;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = tmp;
    cache_cap = new_cap;
}

/* Return the n‑th term of A137199.  The function is memoised so
   subsequent calls are effectively O(1) for already computed values,
   and O(n) for new indices. */
seq_t a(idx_t n)
{
    size_t sn = (size_t)n;      /* use a native integer for array indices */

    /* Initialise the cache on the first call. */
    if (cache_len == 0) {
        ensure_capacity(3);     /* we need indices 0,1,2 at least */
        cache[0] = 1;
        cache[1] = 1;
        cache[2] = 1;
        cache_len = 3;
    }

    /* If the requested index is already known, return it. */
    if (sn < cache_len)
        return cache[sn];

    /* Make sure we have space up to sn. */
    ensure_capacity(sn + 1);

    /* Compute missing values up to sn. */
    for (size_t i = cache_len; i <= sn; ++i) {
        seq_t val;
        if (i < 3) {
            /* The first three values are already set. */
            val = cache[i];
        } else {
            /* Recurrence: a(i) = a(i-1) + 3*a(i-2) + a(i-3) */
            val = cache[i - 1] + 3 * cache[i - 2] + cache[i - 3];
        }
        cache[i] = val;
    }
    cache_len = sn + 1;           /* all values up to sn are ready */
    return cache[sn];
}
