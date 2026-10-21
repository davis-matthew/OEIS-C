/*
 * A074678  a(n) = Sum_{j=0..floor(n/2)} (-1)^(j+floor(n/2))*S(2j+q), where
 *          S(n) are generalized tribonacci numbers (A001644) and q =
 *          (1-(-1)^n)/2.
 * https://oeis.org/A074678
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = a(n-1) + 2*a(n-3) + a(n-4) + a(n-5), a(0)=3, a(1)=1,
 *            a(2)=0, a(3)=6, a(4)=11
 * Range:     u8 n <= 9, u16 n <= 18, u32 n <= 36, u64 n <= 73, u128 n <= 146,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* initial values for n < 5 */
static const seq_t init[5] = { (seq_t)3, (seq_t)1, (seq_t)0, (seq_t)6, (seq_t)11 };

/* cache for computed terms */
static seq_t *cache = NULL;
static size_t cache_len = 0;   /* number of entries currently stored */

/* ensure that cache contains terms up to index 'upto' (inclusive) */
static void ensure_cache(size_t upto)
{
    if (upto < cache_len) /* already computed */
        return;

    /* grow the cache */
    size_t new_len = upto + 1;
    seq_t *new_cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (!new_cache)   /* on allocation failure abort */
        abort();

    cache = new_cache;

    /* compute the missing terms */
    for (size_t i = cache_len; i <= upto; ++i) {
        seq_t val;
        if (i < 5) {
            val = init[i];
        } else {
            /* recurrence: a(n) = a(n-1) + 2*a(n-3) + a(n-4) + a(n-5) */
            val = cache[i - 1] + ((seq_t)2 * cache[i - 3]) + cache[i - 4] + cache[i - 5];
        }
        cache[i] = val;
    }

    cache_len = new_len;
}

seq_t a(idx_t n)
{
    /* convert the unsigned 64‑bit index to size_t for array access. */
    size_t idx = (size_t)n;
    ensure_cache(idx);
    return cache[idx];
}
