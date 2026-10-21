/*
 * A063204  Dimension of the space of weight 2n cuspidal newforms for Gamma_0(
 *          25 ).
 * https://oeis.org/A063204
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-2) + a(n-3) – a(n-5); Initial values: a(1)=0, a(2)=3,
 *            a(3)=7, a(4)=9, a(5)=13
 * Range:     u8 n <= 50, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Static cache: cache[0] holds a(1), cache[1] holds a(2), etc. */
static seq_t *cache   = NULL;   /* cached values */
static idx_t cache_size = 0;    /* number of values already cached */
static idx_t cache_cap  = 0;    /* current capacity of the cache */

/* Ensure the cache can hold at least 'n' values.  */
static void ensure_capacity(idx_t n)
{
    if (cache_cap >= n) return;          /* already large enough */

    idx_t new_cap = cache_cap ? cache_cap * 2 : 64;
    while (new_cap < n) new_cap *= 2;     /* grow exponentially */

    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();                    /* allocation failure */

    /* Initialise any newly allocated elements to zero.  */
    for (idx_t i = cache_size; i < new_cap; ++i)
        tmp[i] = 0;

    cache = tmp;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;

    /* Compute missing values up to n, using the recurrence.  */
    if (cache_size < n) {
        ensure_capacity(n);

        for (idx_t i = cache_size + 1; i <= n; ++i) {
            seq_t val;
            if (i == 1)          val = 0;
            else if (i == 2)     val = 3;
            else if (i == 3)     val = 7;
            else if (i == 4)     val = 9;
            else if (i == 5)     val = 13;
            else                 /* recurrence: a(i-2) + a(i-3) – a(i-5) */
                val = cache[i-3] + cache[i-4] - cache[i-6];

            cache[i-1] = val;    /* store a(i) */
        }
        cache_size = n;
    }
    return cache[n-1];
}
