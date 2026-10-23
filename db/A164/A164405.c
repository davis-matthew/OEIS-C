/*
 * A164405  Number of binary strings of length n with no substrings equal to
 *          0010 or 1100.
 * https://oeis.org/A164405
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) - a(n-3) + a(n-6) for n >= 6 Initial values:
 *            a(0)=1, a(1)=2, a(2)=4, a(3)=8, a(4)=14, a(5)=24
 * Range:     u8 n <= 9, u16 n <= 19, u32 n <= 39, u64 n <= 80, u128 n <= 161,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Stores previously computed terms. */
static seq_t *cache = NULL;
/* Current allocated capacity of the cache (array size). */
static idx_t cache_cap = 0;
/* Number of values currently stored (cache_size = number of valid entries). */
static idx_t cache_size = 0;

/* Base values for indices 0 … 5. */
static const seq_t base[6] = {
    (seq_t)1, (seq_t)2, (seq_t)4, (seq_t)8, (seq_t)14, (seq_t)24
};

/* Ensure that the cache has space for at least (n+1) entries. */
static void ensure_capacity(idx_t n)
{
    if (cache_cap >= n + 1)
        return;
    if (cache_cap == 0)
        cache_cap = 1;
    while (cache_cap <= n)
        cache_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
    cache = tmp;
}

/* Compute the sequence up to index n (inclusive) and store it in the cache. */
static void compute_until(idx_t n)
{
    if (cache_size > n)
        return;

    ensure_capacity(n);

    for (idx_t i = cache_size; i <= n; ++i) {
        if (i < 6) {
            cache[i] = base[i];
        } else {
            /* a(i) = 2*a(i-1) - a(i-3) + a(i-6) */
            cache[i] = (seq_t)2 * cache[i - 1]
                       - cache[i - 3]
                       + cache[i - 6];
        }
    }
    cache_size = n + 1;
}

seq_t a(idx_t n)
{
    compute_until(n);
    return cache[n];
}
