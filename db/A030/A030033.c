/*
 * A030033  a(n+1) = Sum_{k = 0..floor(2*n/3)} a(k)*a(n-k) for n >= 0 with
 *          a(0) = 1.
 * https://oeis.org/A030033
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 15, u32 n <= 28, u64 n <= 52, u128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(n+1)=∑_{k=0}^{⌊2n/3⌋} a(k)·a(n‑k) */

#include <stddef.h>
#include <stdlib.h>

static seq_t *cache = NULL;          /* memoised values */
static size_t cache_cap = 0;         /* capacity of cache array */
static idx_t cache_size = 0;         /* number of values currently stored */

/* enlarge cache to hold at least “needed” elements */
static void grow_cache(size_t needed)
{
    if (cache_cap < needed) {
        size_t new_cap = cache_cap > 0 ? cache_cap * 2 : 1;
        while (new_cap < needed) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    /* initialise cache[0] */
    if (cache_size == 0) {
        grow_cache(1);
        cache[0] = (seq_t)1;
        cache_size = 1;
    }

    /* extend cache up to index n */
    while (cache_size <= n) {
        idx_t m = cache_size;          /* index of value to compute */
        /* n in the recurrence is m-1 */
        idx_t rec_n = m - 1;
        /* floor(2*rec_n/3) computed without overflow */
        idx_t q = rec_n / 3;
        idx_t r = rec_n % 3;
        idx_t bound = q * 2 + (r * 2 / 3);

        seq_t sum = (seq_t)0;
        for (idx_t k = 0; k <= bound; ++k) {
            idx_t j = rec_n - k;
            sum += cache[k] * cache[j];
        }

        grow_cache(cache_size + 1);
        cache[cache_size] = sum;
        ++cache_size;
    }

    return cache[n];
}
