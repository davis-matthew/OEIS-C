/*
 * A180965  Number of tatami tilings of a 2 X n grid (with monomers allowed).
 * https://oeis.org/A180965
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 26, u64 n <= 53, u128 n <= 106,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A180965: number of tatami tilings of a 2 X n grid (with monomers allowed)
   Recurrence: a(n) = 2*a(n-1) + 2*a(n-3) - a(n-4)
   Initial values: a(0)=1, a(1)=2, a(2)=6, a(3)=13
*/

#include <stdlib.h>

/* Caching the computed values so that the function remains fast even for
   non‑monotonic queries. */
static seq_t *cache = NULL;     /* array of computed a(n) */
static size_t cache_size = 0;   /* allocated size of the cache */
static size_t computed_len = 0; /* number of values that are already computed */

/* Ensure that the cache can hold at least 'needed' values. */
static void ensure_cache_capacity(size_t needed)
{
    if (needed <= cache_size)
        return;
    size_t new_size = cache_size ? cache_size : 4;
    while (new_size < needed)
        new_size <<= 1;          /* double until enough */

    seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!tmp)
        abort();                 /* out of memory */

    cache     = tmp;
    cache_size = new_size;
}

/* Initialise the cache with the first four known terms. */
static void init_cache(void)
{
    if (cache != NULL)
        return;

    ensure_cache_capacity(4);
    cache[0] = (seq_t)1;
    cache[1] = (seq_t)2;
    cache[2] = (seq_t)6;
    cache[3] = (seq_t)13;
    computed_len = 4;
}

seq_t a(idx_t n)
{
    init_cache();

    if (n < computed_len)
        return cache[(size_t)n];

    size_t limit = (size_t)n;          /* n is 0‑based index */
    ensure_cache_capacity(limit + 1);  /* +1 because indices are zero‑based */

    for (size_t i = computed_len; i <= limit; ++i)
    {
        /* Recurrence: a(i) = 2*a(i-1) + 2*a(i-3) - a(i-4) */
        seq_t val = cache[i - 1];
        val += cache[i - 1];        /* 2*a(i-1) */
        val += cache[i - 3];
        val += cache[i - 3];        /* 2*a(i-3) */
        val -= cache[i - 4];        /* -a(i-4) */
        cache[i] = val;
    }

    computed_len = limit + 1;
    return cache[limit];
}
