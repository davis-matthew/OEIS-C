/*
 * A008819  Expansion of (1+2*x^5+x^8)/((1-x^2)^2*(1-x^8)).
 * https://oeis.org/A008819
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 61, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A008819: Expansion of (1+2*x^5+x^8)/((1-x^2)^2*(1-x^8)).
   Recurrence:  a(n) = a(n-1)+a(n-2)-a(n-3)+a(n-8)-a(n-9)-a(n-10)+a(n-11)
   Initial values: a(0)=1, a(1)=0, a(2)=2, a(3)=0, a(4)=3, a(5)=2,
   a(6)=4, a(7)=4, a(8)=7, a(9)=6, a(10)=10, a(11)=8. */

#include <stdlib.h>

static seq_t *cache = NULL;      /* memoized values */
static idx_t  cache_len = 0;     /* number of computed entries (cache_len == n+1 after compute) */

/* static array of initial terms up to a(11) */
static const seq_t init_vals[12] = {
    (seq_t)1, 0, 2, 0, 3, 2, 4, 4, 7, 6, 10, 8
};

seq_t a(idx_t n)
{
    /* Ensure cache has space for n+1 entries */
    if (cache_len <= n) {
        size_t new_len = (size_t)(n + 1);
        seq_t *new_cache;
        if (cache == NULL) {
            new_cache = (seq_t *)malloc(new_len * sizeof(seq_t));
        } else {
            new_cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        }
        if (!new_cache) {
            /* Allocation failed – return 0 as a safe fallback */
            return (seq_t)0;
        }
        cache = new_cache;

        /* Initialize missing entries up to new_len-1 */
        idx_t i;
        for (i = cache_len; i <= n; ++i) {
            if (i <= 11) {
                cache[i] = init_vals[i];
            } else {
                /* Recurrence:
                   a(i) = a(i-1)+a(i-2)-a(i-3)+a(i-8)-a(i-9)-a(i-10)+a(i-11) */
                seq_t sum = cache[i-1] + cache[i-2] + cache[i-8] + cache[i-11];
                seq_t sub = cache[i-3] + cache[i-9] + cache[i-10];
                cache[i] = sum - sub;
            }
        }
        cache_len = n + 1;
    }

    return cache[n];
}
