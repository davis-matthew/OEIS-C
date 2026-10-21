/*
 * A229830  Trajectory of 1 under the morphism 1 -> 12, 2 -> 1232, 3 ->
 *          123432, 4 -> 12345432, etc.
 * https://oeis.org/A229830
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8558, u16 n <= 8558, u32 n <= 8558, u64 n <= 8558,
 *            u128 n <= 8558, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function computes the n‑th element of OEIS sequence A229830:
   the trajectory of 1 under the morphism
     1 → 12
     2 → 1232
     3 → 123432
     4 → 12345432,
   etc.  The implementation generates the word iteratively until
   it contains at least n chars, caching the result for later calls. */

#include <stddef.h>
#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_len = 0;

static seq_t* alloc_array(size_t n)
{
#ifdef __cplusplus
    return new seq_t[n];
#else
    return (seq_t*)malloc(n * sizeof(seq_t));
#endif
}

static void free_array(seq_t *p)
{
#ifdef __cplusplus
    delete[] p;
#else
    free(p);
#endif
}

static void expand_cache_to(idx_t needed)
{
    if (cache_len >= needed) return;
    /* initialise with the starting word "1" */
    if (cache_len == 0) {
        cache = alloc_array(1);
        cache[0] = 1;
        cache_len = 1;
    }
    while (cache_len < needed) {
        /* determine length of the next expansion */
        idx_t new_len = 0;
        for (idx_t i = 0; i < cache_len; ++i) {
            idx_t d = (idx_t)cache[i];
            new_len += 2 * d;
        }
        seq_t *new_arr = alloc_array((size_t)new_len);
        idx_t pos = 0;
        for (idx_t i = 0; i < cache_len; ++i) {
            size_t d = (size_t)cache[i];
            /* ascending part 1 … d+1 */
            for (size_t x = 1; x <= d + 1; ++x)
                new_arr[pos++] = (seq_t)x;
            /* descending part d … 2 */
            for (size_t x = d; x >= 2; --x)
                new_arr[pos++] = (seq_t)x;
        }
        free_array(cache);
        cache = new_arr;
        cache_len = new_len;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;      /* offset is 1; n<1 is undefined */
    expand_cache_to(n);
    return cache[n - 1];
}
