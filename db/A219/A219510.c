/*
 * A219510  Pierce expansion of 84 - 48*sqrt(3).
 * https://oeis.org/A219510
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=7; for n>=1: a(2n)=4*(a(2n-1)+1);
 *            a(2n+1)=2*(a(2n-1))^2-1
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 10, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;   /* memoised terms */
static idx_t cache_len = 0;   /* number of cached terms */

/* Compute terms up to index n, extending the cache as necessary. */
static void ensure_upto(idx_t n)
{
    if (cache_len > n) return;          /* already computed */

    idx_t new_len = n + 1;
    seq_t *new_cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (!new_cache) return;              /* allocation failed – caller gets 0 */
    cache = new_cache;
    cache_len = new_len;

    /* start filling from the last cached value */
    for (idx_t i = 0; i < cache_len; ++i) {
        if (i == 0) {
            cache[0] = (seq_t)1;
        } else if (i == 1) {
            cache[1] = (seq_t)7;
        } else if ((i & 1) == 0) {   /* even */
            seq_t prev = cache[i - 1];
            cache[i] = (seq_t)4 * (prev + (seq_t)1);
        } else {                     /* odd, i >= 3 */
            seq_t prev = cache[i - 2];
            cache[i] = (seq_t)2 * prev * prev - (seq_t)1;
        }
    }
}

seq_t a(idx_t n)
{
    ensure_upto(n);
    if (n < cache_len) return cache[n];
    return (seq_t)0;   /* should not happen */
}
