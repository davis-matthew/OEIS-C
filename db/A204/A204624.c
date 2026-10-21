/*
 * A204624  Number of (n+1) X 3 0..2 arrays with every 2 X 2 subblock having
 *          unequal diagonal elements or unequal antidiagonal elements, and
 *          new values 0..2 introduced in row major order.
 * https://oeis.org/A204624
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 22*a(n-1) - 14*a(n-2) (n ≥ 3) Initial values: a(1) = 96,
 *            a(2) = 2049
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 14, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Cached values: cache[0] == a(1), cache[1] == a(2), ...   */
static seq_t *cache = NULL;
static idx_t cache_size = 0;   /* number of terms currently stored */

/* Allocate or grow the cache so that it contains terms up to and
   including a(n). New terms are computed using the recurrence. */
static void grow_cache(idx_t n)
{
    if (n <= cache_size)
        return;

    /* Reallocate cache to hold a(1)…a(n) */
    seq_t *tmp = (seq_t *)realloc(cache, n * sizeof(seq_t));
    if (!tmp)
        return;               /* allocation failed – leave cache unchanged */

    cache = tmp;

    /* Ensure the first two terms are initialized */
    if (n >= 1)
        cache[0] = (seq_t)96;    /* a(1) */
    if (n >= 2)
        cache[1] = (seq_t)2049;  /* a(2) */

    /* Compute any additional required terms */
    for (idx_t i = (cache_size < 2 ? 3 : cache_size + 1); i <= n; ++i) {
        seq_t a_prev      = cache[i - 2];   /* a(i-1) */
        seq_t a_prevprev  = cache[i - 3];   /* a(i-2) */
        cache[i - 1] = a_prev * (seq_t)22 - a_prevprev * (seq_t)14;
    }

    cache_size = n;
}

/* Return a(n) for the OEIS sequence A204624.  For n < 1 the function
   returns 0 as a placeholder. */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;   /* undefined for indices below the offset */

    grow_cache(n);
    return cache[n - 1];
}
