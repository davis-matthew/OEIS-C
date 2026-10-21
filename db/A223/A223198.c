/*
 * A223198  Rolling cube footprints: number of n X 4 0..5 arrays starting with
 *          0 where 0..5 label faces of a cube and every array movement to a
 *          horizontal or vertical neighbor moves across a corresponding cube
 *          edge.
 * https://oeis.org/A223198
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 144*a(n-1) - 3840*a(n-2) + 24576*a(n-3) Initial values:
 *            a(1)=64, a(2)=6912, a(3)=765952
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 9, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, realloc */

static seq_t *cache = NULL;      /* cache[0] = a(1), cache[1] = a(2), ... */
static size_t cache_len = 0;     /* number of terms currently stored */

/* Ensure cache contains the first `n` terms of the sequence. */
static void ensure_cache(idx_t n)
{
    if (n == 0)
        return;                 /* nothing to do for nonpositive n */

    size_t target = (size_t)n;
    if (target <= cache_len)
        return;                 /* already have enough terms */

    /* Allocate or grow the cache */
    seq_t *new_cache = (seq_t *)realloc(cache, target * sizeof(seq_t));
    if (!new_cache) abort();    /* allocation failure */
    cache = new_cache;

    /* Compute any new terms that are missing */
    for (size_t idx = cache_len; idx < target; ++idx) {
        idx_t k = (idx_t)(idx + 1);   /* 1‑based index */
        seq_t val;
        if (k == 1) {
            val = (seq_t)64;
        } else if (k == 2) {
            val = (seq_t)6912;
        } else if (k == 3) {
            val = (seq_t)765952;
        } else {
            /* Recurrence: a(n) = 144*a(n-1) - 3840*a(n-2) + 24576*a(n-3) */
            val = (seq_t)144 * cache[idx - 1]          /* a(n-1) */
                - (seq_t)3840 * cache[idx - 2]          /* a(n-2) */
                + (seq_t)24576 * cache[idx - 3];         /* a(n-3) */
        }
        cache[idx] = val;
    }

    cache_len = target;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* undefined for n=0; return 0 */
    ensure_cache(n);
    return cache[(size_t)(n - 1)];
}
