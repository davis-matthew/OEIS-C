/*
 * A080926  Partial sums of A080925.
 * https://oeis.org/A080926
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 40, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A080926 – Partial sums of A080925
   Recurrence: a(0)=0, a(1)=1, a(2)=6,
               a(n)=3*a(n‑1)+a(n‑2)-3*a(n‑3)  (for n>2)           */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache ----------------------------------------------------- */
    static seq_t *cache = NULL; /* window of previously computed values   */
    static idx_t cache_len = 0; /* number of values stored (valid 0..cache_len‑1) */

    /* early return if value already cached */
    if (n < cache_len)
        return cache[n];

    /* enlarge cache to accommodate indices up to n */
    idx_t new_len = n + 1;
    if (!cache) {
        cache = (seq_t *)malloc(new_len * sizeof(seq_t));
        if (!cache) return (seq_t)0;            /* allocation failure – safe fall‑back */
    } else {
        cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!cache) return (seq_t)0;            /* allocation failure – safe fall‑back */
    }

    /* fill in missing terms using the recurrence */
    for (idx_t i = cache_len; i <= n; ++i) {
        if (i == 0)            cache[i] = (seq_t)0;
        else if (i == 1)       cache[i] = (seq_t)1;
        else if (i == 2)       cache[i] = (seq_t)6;
        else                   cache[i] = (seq_t)3 * cache[i-1]
                                + cache[i-2]
                                - (seq_t)3 * cache[i-3];
    }

    cache_len = new_len;  /* update cache size */
    return cache[n];
}
