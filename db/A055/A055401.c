/*
 * A055401  Number of positive cubes needed to sum to n using the greedy
 *          algorithm.
 * https://oeis.org/A055401
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0)=0; for n≥1, a(n)=1+ a(n - k³) where k = ⌊∛n⌋. * Implements
 *            the greedy algorithm for expressing n as a sum of cubes * and
 *            counts the number of cubes used
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;      /* memoisation table for a(n) values */
static idx_t cache_size = 0;     /* current allocated size of cache */

/* Compute the integer cube‑root floor of n.
 * Uses binary search with 128‑bit arithmetic to avoid overflow. */
static idx_t cube_root_floor(idx_t n)
{
    if (n == 0) return 0;
    idx_t low = 0, high = 1;
    /* increase high until high³ > n */
    while ((unsigned __int128)high * high * high <= n) high <<= 1;
    /* binary search in (low, high] */
    while (high - low > 1) {
        idx_t mid = low + (high - low) / 2;
        unsigned __int128 cube = (unsigned __int128)mid * mid * mid;
        if (cube <= n) low = mid;
        else high = mid;
    }
    return low;
}

/* Ensure the cache can hold indices up to n.  Allocates or expands
 * the cache as necessary, copying existing values via assignment
 * (works for POD and non‑POD seq_t). */
static void ensure_cache(idx_t n)
{
    if (n < cache_size) return;

    idx_t new_size = cache_size > 0 ? cache_size : 1;
    while (new_size <= n) new_size <<= 1;

    seq_t *new_arr = (seq_t *)calloc(new_size, sizeof(seq_t));
    if (cache) {
        for (idx_t i = 0; i < cache_size; ++i) new_arr[i] = cache[i];
        free(cache);
    }
    cache = new_arr;
    cache_size = new_size;
}

/* Recursive helper with memoisation.  a_rec(n) returns the number
 * of cubes needed to represent n.  (n==0 is the base case.) */
static seq_t a_rec(idx_t n)
{
    if (n == 0) return 0;

    if (n < cache_size && cache[n] != 0) return cache[n];

    idx_t k = cube_root_floor(n);
    idx_t sub = n - (idx_t)((unsigned __int128)k * k * k);
    seq_t res = a_rec(sub) + 1;

    ensure_cache(n);
    cache[n] = res;
    return res;
}

/* Public interface as required. */
seq_t a(idx_t n)
{
    return a_rec(n);
}
