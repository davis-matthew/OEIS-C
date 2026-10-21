/*
 * A095681  Expansion of 1/((1-x)(1-x-x^2)(1-x-x^2-x^3)).
 * https://oeis.org/A095681
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 32, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A095681: Expansion of 1/((1-x)(1-x-x^2)(1-x-x^2-x^3)).
   Recurrence: a(n) = 3*a(n-1) - a(n-2) - 2*a(n-3) - a(n-4) + a(n-5) + a(n-6)
   Initial values: a(0)=1, a(1)=3, a(2)=8, a(3)=19, a(4)=42, a(5)=89
*/
#include <stdlib.h>

static seq_t *cache = NULL;      /* dynamic array storing computed terms   */
static size_t cache_cap = 0;     /* current allocated capacity (in seq_t)  */
static idx_t cache_len = 0;      /* number of terms that have been computed */

seq_t a(idx_t n)
{
    /* Grow the cache to hold at least n+1 terms, reserving space for
       the first six terms to avoid overflow when initializing them. */
    size_t new_cap = (size_t)(n + 1);
    if (new_cap < 6) new_cap = 6;

    if (new_cap > cache_cap) {
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Initialise the first six terms only once. */
    if (cache_len == 0) {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)3;
        cache[2] = (seq_t)8;
        cache[3] = (seq_t)19;
        cache[4] = (seq_t)42;
        cache[5] = (seq_t)89;
        cache_len = 6;
    }

    /* Compute missing terms up to a(n). */
    if ((size_t)(n + 1) > cache_len) {
        for (idx_t i = cache_len; i <= n; ++i) {
            seq_t val = (seq_t)3 * cache[i - 1]
                        - cache[i - 2]
                        - (seq_t)2 * cache[i - 3]
                        - cache[i - 4]
                        + cache[i - 5]
                        + cache[i - 6];
            cache[i] = val;
        }
        cache_len = n + 1;
    }

    return cache[n];
}
