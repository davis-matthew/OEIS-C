/*
 * A158943  INVERT transform of A027656: (1, 0, 2, 0, 3, 0, 4, 0, 5, ...).
 * https://oeis.org/A158943
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 2*a(n-2) - a(n-4) with initial values a(1)=1,
 *            a(2)=1, a(3)=3, a(4)=5. This implements the recurrence of OEIS
 *            sequence A158943
 * Range:     u8 n <= 10, u16 n <= 18, u32 n <= 35, u64 n <= 70, u128 n <= 139,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;      /* 1‑based cache of computed terms */
static idx_t cache_size = 0;     /* number of stored terms plus one (index 0 unused) */

/* Ensure that cache covers terms up to n (inclusive). */
static void ensure_cache(idx_t n)
{
    if (n + 1 <= cache_size) return;          /* already covered */

    /* Grow the cache to hold terms up to n. */
    size_t new_size = (size_t)(n + 1);
    /* realloc grows or creates memory; keep indices 0..n */
    seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!tmp) abort();                         /* out of memory */

    cache = tmp;

    /* Fill in new terms beyond previous size. */
    for (idx_t i = cache_size; i <= n; ++i)
    {
        if (i == 1)            cache[i] = (seq_t)1;
        else if (i == 2)       cache[i] = (seq_t)1;
        else if (i == 3)       cache[i] = (seq_t)3;
        else if (i == 4)       cache[i] = (seq_t)5;
        else
        {
            /* a(i) = a(i-1) + 2*a(i-2) - a(i-4) */
            cache[i] = cache[i-1] + cache[i-2] * (seq_t)2 - cache[i-4];
        }
    }
    cache_size = new_size;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined for offset 1, return 0 for safety */
    ensure_cache(n);
    return cache[n];
}
