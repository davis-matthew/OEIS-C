/*
 * A170085  Number of reduced words of length n in Coxeter group on 28
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^37 = I.
 * https://oeis.org/A170085
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
   a(n) = 26*(a(n-1)+…+a(n-36)) - 351*a(n-37) + p(n)
   where   p(0)=1,
           p(n)=2 for 1≤n≤36,
           p(37)=1,
           p(n)=0 for n>37.
   For n≥37 (p(n)=0) this reduces to the homogeneous linear recurrence.  */

/* Offsets: a(0)=1, a(1)=28, a(2)=756, …  */

#include <stdlib.h>
#include <stdint.h>

/* The types seq_t and idx_t are supplied by the build system. */
static seq_t *cache = NULL;        /* array of computed terms */
static idx_t cache_size = 0;       /* number of terms currently stored (up to cache_size-1) */
static idx_t cache_cap  = 0;       /* allocated capacity of 'cache' */

/* Ensure that terms up to index 'n' (inclusive) are computed. */
static void
ensure_cache(idx_t n)
{
    if (n < cache_size) return;          /* nothing to do */

    /* Grow the storage if needed. */
    if (cache_cap <= n) {
        idx_t new_cap = cache_cap ? cache_cap << 1 : 64;
        while (new_cap <= n) new_cap <<= 1;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* Compute all missing terms. */
    for (idx_t i = cache_size; i <= n; ++i) {
        seq_t sum = 0;

        /* Add 26*(a(i-1)+…+a(i-36)), stopping at k_max = min(i,36). */
        idx_t k_max = i < 36 ? i : 36;
        for (idx_t k = 1; k <= k_max; ++k) {
            sum += (seq_t)26 * cache[i - k];
        }

        /* Subtract 351*a(i-37) if i≥37. */
        if (i >= 37) {
            sum -= (seq_t)351 * cache[i - 37];
        }

        /* Add the inhomogeneous term p(i). */
        seq_t p = 0;
        if (i == 0)         p = 1;
        else if (i <= 36)   p = 2;
        else if (i == 37)   p = 1;          /* otherwise p stays 0 */

        sum += p;
        cache[i] = sum;
    }

    cache_size = n + 1;
}

/* Public API: compute a(n). */
seq_t
a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
