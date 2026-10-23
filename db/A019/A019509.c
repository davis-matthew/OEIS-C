/*
 * A019509  Nim-values for the impartial game Take-a-Triangle.
 * https://oeis.org/A019509
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Nim-values for the impartial game Take‑a‑Triangle.
   a(n) = mex{ a(n - k(k+1)/2) : k ≥ 1 , k(k+1)/2 ≤ n }  */
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/* cache of previously computed nim‑values */
static seq_t *cache = NULL;
static idx_t cached_len = 0;

/* helper array used to compute the mex; it grows as needed */
static bool *used = NULL;
static size_t used_cap = 0;

/* auxiliary storage for reachable nim‑values */
#define TEMP_CAP 512
static seq_t temp[TEMP_CAP];

/* ------------------------------------------------------------------ */
/* compute a(n) for all n ≤ given value                                         */
/* ------------------------------------------------------------------ */
static void ensure_cache(idx_t n)
{
    if (n < cached_len)
        return;                           /* nothing to do */

    /* enlarge cache */
    size_t new_cap = (size_t)(n + 1);
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache) /* allocation failed – return zero as a fallback */
        return;
    cache = new_cache;

    /* initialise first entry if the cache was empty */
    if (cached_len == 0)
        cache[0] = (seq_t)0;

    /* compute new entries */
    for (idx_t i = cached_len; i <= n; ++i) {
        /* collect reachable nim‑values into temp[ ] and find maximal value  */
        idx_t k = 1, j = 0;
        seq_t max_val = (seq_t)0;
        while (1) {
            idx_t t = k * (k + 1) / 2; /* triangular number */
            if (t > i)
                break;
            seq_t v = cache[i - t];
            if (j < TEMP_CAP)
                temp[j++] = v;
            if (v > max_val)
                max_val = v;
            ++k;
        }

        /* enlarge used array to contain at least max_val */
        size_t need = (size_t)max_val + 1;
        if (used_cap < need) {
            bool *new_used = (bool *)realloc(used, need);
            if (!new_used) /* allocation failed – fall back to 0 */
                return;
            used = new_used;
            used_cap = need;
        }

        /* clear used array */
        memset(used, 0, used_cap * sizeof(bool));

        /* mark reachables */
        for (size_t z = 0; z < (size_t)j; ++z) {
            size_t idx = (size_t)temp[z];
            if (idx < used_cap)
                used[idx] = true;
        }

        /* find mex */
        seq_t mex = (seq_t)0;
        while ((size_t)mex < used_cap && used[(size_t)mex])
            ++mex;
        cache[i] = mex;
    }

    cached_len = n + 1;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    if (n < cached_len)
        return cache[n];
    return (seq_t)0;   /* fallback on allocation failure */
}
