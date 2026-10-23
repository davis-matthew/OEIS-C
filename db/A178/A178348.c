/*
 * A178348  a(n) = Sum_{k=0..n} 1100^k.
 * https://oeis.org/A178348
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..n} 1100^k Recurrence: a(0)=1;
 *            a(n)=1100*a(n-1)+1 for n>=1
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 3, u64 n <= 6, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* External types defined elsewhere */
extern seq_t a(idx_t n);

/* Internal static cache used to memoise results */
static seq_t *cache = NULL;
static idx_t cache_len = 0;

/* Compute a(n) with memoisation */
seq_t a(idx_t n)
{
    /* Ensure cache[0] is initialised */
    if (cache_len == 0) {
        /* Allocate one element for a(0) */
        cache = (seq_t*)malloc(sizeof(seq_t));
        if (!cache) { /* OOM – return 0 as fallback */ return (seq_t)0; }
        cache[0] = (seq_t)1;
        cache_len = 1;
    }

    /* If requested index is beyond current cache size, grow and compute */
    if (n >= cache_len) {
        idx_t new_len = n + 1;
        /* Reallocate cache to hold new_len elements */
        seq_t *tmp = (seq_t*)realloc(cache, sizeof(seq_t) * (size_t)new_len);
        if (!tmp) { /* OOM – return 0 as fallback */ return (seq_t)0; }
        cache = tmp;

        /* Compute values from the first missing index up to n */
        for (idx_t i = cache_len; i <= n; ++i) {
            cache[i] = cache[i - 1] * (seq_t)1100 + (seq_t)1;
        }
        cache_len = new_len;
    }

    return cache[n];
}
