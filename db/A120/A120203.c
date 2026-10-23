/*
 * A120203  a(1) = 2; a(n) = floor( (19 + Sum_{i=1..n-1} a(i)) /9).
 * https://oeis.org/A120203
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 2; a(n) = floor((19 + Sum_{i=1..n-1} a(i)) / 9)
 * Range:     u8 n <= 28, u16 n <= 20, u32 n <= 10, u64 n <= 57, u128 n <= 4,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (OEIS A120203) */

#include <stdlib.h>
#include <string.h>

/* static cache for computed terms */
static seq_t *cache = NULL;
static idx_t cache_size = 0;           /* number of allocated slots (index 0 unused) */
static idx_t last_computed = 0;        /* highest index that has been computed */
static seq_t cumulative_sum = 0;       /* Sum_{i=1..last_computed} a(i) */

/* Expand cache to hold indices up to 'needed' (inclusive) */
static void expand_cache(idx_t needed)
{
    if (needed <= cache_size)
        return;
    idx_t new_size = needed + 1; /* indices 0..needed */
    seq_t *new_arr = (seq_t *)malloc(new_size * sizeof(seq_t));
    if (!new_arr)
        return;                 /* allocation failure – leave existing cache */
    if (cache) {
        memcpy(new_arr, cache, cache_size * sizeof(seq_t));
        free(cache);
    }
    if (new_size > cache_size)
        memset(new_arr + cache_size, 0, (new_size - cache_size) * sizeof(seq_t));
    cache = new_arr;
    cache_size = new_size;
}

/* OEIS sequence A120203 */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;               /* undefined for n=0; harmless return */

    if (n >= cache_size)
        expand_cache(n);

    if (n > last_computed) {
        /* compute terms from last_computed+1 up to n */
        for (idx_t i = last_computed + 1; i <= n; ++i) {
            seq_t ai = ((seq_t)19 + cumulative_sum) / (seq_t)9;
            cache[i] = ai;
            cumulative_sum += ai;
        }
        last_computed = n;
    }
    return cache[n];
}
