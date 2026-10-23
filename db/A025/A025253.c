/*
 * A025253  a(n) = a(1)*a(n-1) + a(2)*a(n-2) + ... + a(n-3)*a(3) for n >= 4,
 *          with initial terms 0, 2, 1, 0.
 * https://oeis.org/A025253
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 21, u32 n <= 36, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=0, a(2)=2, a(3)=1, a(4)=0,
   a(n)=∑_{i=1}^{n-3} a(i)*a(n-i)  for n≥4   (OEIS A025253) */

#include <stdlib.h>

/* static cache for memoisation */
static seq_t *seq_cache = NULL;
static size_t cache_cap   = 0;   /* capacity of the cache, indices 0..cache_cap-1 */
static idx_t  cache_len   = 0;   /* first unused index (highest computed + 1) */

/* Ensure the cache has at least 'needed' slots (0..needed-1). */
static void ensure_stored_capacity(size_t needed)
{
    if (cache_cap >= needed) return;

    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    if (new_cap < needed) new_cap = needed;

    seq_t *tmp = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
    if (!tmp) abort(); /* allocation failure */

    /* Zero‑initialise the newly allocated part */
    for (size_t i = cache_cap; i < new_cap; ++i) tmp[i] = 0;

    seq_cache = tmp;
    cache_cap = new_cap;
}

/* Initialise the first four non‑zero terms on first use. */
static void init_cache(void)
{
    ensure_stored_capacity(5);   /* indices 0..4 */
    seq_cache[0] = 0;
    seq_cache[1] = 0;   /* a(1) */
    seq_cache[2] = 2;   /* a(2) */
    seq_cache[3] = 1;   /* a(3) */
    seq_cache[4] = 0;   /* a(4) */
    cache_len = 5;      /* indices 0..4 are now valid */
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;

    if (cache_len == 0) init_cache();

    /* Need storage up to index n */
    ensure_stored_capacity((size_t)n + 1);

    if (n < cache_len) return seq_cache[(size_t)n];

    /* Compute missing values up to n */
    for (idx_t m = cache_len; m <= n; ++m)
    {
        seq_t val;
        if (m <= 4)
        {
            /* already initialised, but keep for clarity */
            val = seq_cache[(size_t)m];
        }
        else
        {
            val = 0;
            for (idx_t i = 1; i <= m - 3; ++i)
                val += seq_cache[(size_t)i] * seq_cache[(size_t)(m - i)];
        }
        seq_cache[(size_t)m] = val;
    }

    cache_len = (size_t)n + 1;
    return seq_cache[(size_t)n];
}
