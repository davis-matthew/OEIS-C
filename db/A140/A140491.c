/*
 * A140491  Trajectory of 2 under repeated application of the map: n -> n +
 *          third-smallest number that does not divide n.
 * https://oeis.org/A140491
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=2, a(2)=7, a(3)=11, a(4)=15, a(5)=21, a(6)=26, a(7)=31, *
 *            a(8)=35, a(9)=39, a(10)=44, a(11)=50, a(12)=56, a(13)=62. * For
 *            n > 13: a(n) = a(n‑1) + a(n‑12) – a(n‑13)
 * Range:     u8 n <= 52, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* --- Memoization cache ------------------------------------------------- */
static seq_t  *cache      = NULL; /* array of terms, 1‑based indices         */
static size_t  cache_cap  = 0;    /* allocated capacity (number of slots)   */
static size_t  cache_used = 0;    /* highest index that has a defined value */

/* Ensure that cache has room for indices 0 .. idx (inclusive). */
static void grow_cache_to(size_t idx)
{
    if (idx < cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap << 1 : 16;
    while (new_cap <= idx)
        new_cap <<= 1;
    cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache_cap = new_cap;
}

/* Initialise the first 13 terms (indices 1 … 13). */
static void init_cache(void)
{
    static const idx_t init_vals[13] = {
        2, 7, 11, 15, 21, 26, 31, 35, 39, 44, 50, 56, 62
    };
    grow_cache_to(13);                      /* need indices 0 … 13 */
    for (size_t i = 1; i <= 13; ++i)
        cache[i] = (seq_t)init_vals[i-1];    /* 1‑based storage */
    cache_used = 13;                        /* last defined index */
}

/* ---------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)                 /* undefined, but keep behaviour defined */
        return (seq_t)0;

    size_t idx = (size_t)n;     /* 1‑based index for array access */

    if (cache_used == 0)        /* first call */
        init_cache();

    if (idx <= cache_used)      /* already known */
        return cache[idx];

    /* Compute all missing terms up to idx inclusive. */
    grow_cache_to(idx);         /* ensure array large enough */

    for (size_t i = cache_used + 1; i <= idx; ++i) {
        /* a(i) = a(i-1) + a(i-12) – a(i-13)  */
        /* All referenced indices are already computed above. */
        seq_t val = cache[i-1] + cache[i-12] - cache[i-13];
        cache[i] = val;
    }
    cache_used = idx;
    return cache[idx];
}
