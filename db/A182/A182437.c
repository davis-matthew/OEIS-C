/*
 * A182437  a(0)=0, a(n) = (a(n-1) XOR n) * (n+1).
 * https://oeis.org/A182437
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(0)=0, a(n) = (a(n-1) XOR n) * (n+1) for n>0
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 19, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;   /* cache of already computed values */
static idx_t cached_upto = 0; /* highest n for which cache[n] is valid */

/* Compute the n-th term of A182437, memoizing intermediate results. */
seq_t a(idx_t n)
{
    /* Ensure the cache exists and holds a[0] = 0. */
    if (cache == NULL) {
        cache = (seq_t*)malloc(sizeof(seq_t));
        if (!cache) abort();   /* allocation failure - abort is safe */
        cache[0] = 0;
        cached_upto = 0;
    }

    /* Grow the cache if the required index is beyond the current limit. */
    if (n > cached_upto) {
        size_t new_size = (size_t)n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) abort();    /* allocation failure - abort is safe */
        cache = tmp;

        for (idx_t i = cached_upto + 1; i <= n; ++i) {
            /* recurrence: a(i) = (a(i-1) XOR i) * (i+1) */
            cache[i] = ((cache[i - 1] ^ (seq_t)i) * ((seq_t)i + 1));
        }
        cached_upto = n;
    }

    return cache[n];
}
