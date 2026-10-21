/*
 * A085717  Consider the square lattice L and the sublattice K of index 5
 *          spanned by (2,-1), (1,2); a(n) = number of points (x,y) in M with
 *          x >= 0, y >= 0, x+y <= n.
 * https://oeis.org/A085717
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = coefficient of x^n in (x^3 - x + 1)/((1 - x^5)(1 - x)^2)
 *            Recurrence: a(n) = 2a(n-1) - a(n-2) + a(n-5) - 2a(n-6) + a(n-7)
 *            (n ≥ 7) Initial values: a(0)=1, a(1)=1, a(2)=1, a(3)=2, a(4)=3,
 *            a(5)=5, a(6)=6
 * Range:     u8 n <= 49, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;          /* cached values of the sequence   */
static size_t cache_len = 0;         /* number of entries currently cached */

/* Initialise the cache with the first seven terms. */
static void init_cache(void)
{
    if (cache_len != 0)
        return;

    cache_len = 7;
    cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
    if (cache == NULL)
        abort();  /* allocation failure */

    cache[0] = (seq_t)1;  /* a(0) */
    cache[1] = (seq_t)1;  /* a(1) */
    cache[2] = (seq_t)1;  /* a(2) */
    cache[3] = (seq_t)2;  /* a(3) */
    cache[4] = (seq_t)3;  /* a(4) */
    cache[5] = (seq_t)5;  /* a(5) */
    cache[6] = (seq_t)6;  /* a(6) */
}

/* Extend the cache so that it contains values up to index n (inclusive). */
static void ensure_cache(idx_t n)
{
    init_cache();

    if ((size_t)n + 1 <= cache_len)
        return;

    size_t new_len = (size_t)n + 1;
    seq_t *new_cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (new_cache == NULL)
        abort();  /* allocation failure */

    cache = new_cache;

    /* Compute new terms using the recurrence. */
    for (size_t i = cache_len; i <= (size_t)n; ++i) {
        /* For i >= 7 use the recurrence; for safety the loop only reaches i>=7. */
        cache[i] = (seq_t)2 * cache[i-1]         /* 2 a(i-1)   */
                 - cache[i-2]                   /* - a(i-2)   */
                 + cache[i-5]                   /* + a(i-5)   */
                 - (seq_t)2 * cache[i-6]        /* - 2 a(i-6) */
                 + cache[i-7];                  /* + a(i-7)   */
    }

    cache_len = new_len;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[(size_t)n];
}
