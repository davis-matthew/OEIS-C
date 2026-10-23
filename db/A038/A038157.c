/*
 * A038157  a(n) = n! * Sum_{k=1..n-2} 1/k!.
 * https://oeis.org/A038157
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = n! * Sum_{k=1..n-2} 1/k!
 * Recurrence: a(0)=a(1)=a(2)=0; for n>=3: a(n) = n*a(n-1) + n*(n-1)
 */

#include <stdlib.h>

static seq_t *a_cache = NULL;   /* cache of computed values */
static idx_t      a_cache_len = 0;   /* current length of the cache (number of indices stored) */
static idx_t      a_cache_cap = 0;   /* allocated capacity of the cache */

/* Ensure that values up to index n are computed and stored in the cache. */
static void a_cache_ensure(idx_t n)
{
    if (a_cache_len > n)
        return;   /* already computed */

    /* expand capacity if necessary */
    if (a_cache_cap <= n) {
        size_t new_cap = n + 1;
        if (new_cap < 8)  /* start with a small default */
            new_cap = 8;
        seq_t *tmp = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
        a_cache   = tmp;
        a_cache_cap = new_cap;
    }

    /* compute missing terms */
    for (idx_t i = a_cache_len; i <= n; ++i) {
        if (i <= 2) {
            a_cache[i] = 0;
        } else {
            /* a(i) = i*a(i-1) + i*(i-1)  (integer arithmetic only) */
            seq_t prod = (seq_t)i * a_cache[i-1];
            prod += (seq_t)i * (i-1);
            a_cache[i] = prod;
        }
    }
    a_cache_len = n + 1;
}

seq_t a(idx_t n)
{
    a_cache_ensure(n);
    return a_cache[n];
}
