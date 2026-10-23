/*
 * A155460  a(n) = 8*a(n-1)+49*a(n-2), n>2 ; a(0)=1, a(1)=1, a(2)=8 .
 * https://oeis.org/A155460
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 8*a(n-1) + 49*a(n-2), with a(0)=1, a(1)=1, a(2)=8
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t capacity = 0;
static idx_t max_cached = 0;

seq_t a(idx_t n)
{
    /* handle the three initial terms directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)8;

    /* lazy initialization of the cache */
    if (!cache) {
        capacity = 3;
        cache = (seq_t *)malloc(capacity * sizeof(seq_t));
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)8;
        max_cached = 2;
    }

    /* if the requested value is already cached, return it */
    if (n <= max_cached) {
        return cache[n];
    }

    /* grow the cache if needed */
    if (n >= capacity) {
        idx_t new_capacity = n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_capacity * sizeof(seq_t));
        if (!new_cache) {
            /* allocation failed – compute without caching */
            seq_t a0 = (seq_t)1, a1 = (seq_t)1, a2 = (seq_t)8, next;
            for (idx_t i = 3; i <= n; ++i) {
                next = ((seq_t)8 * a2) + ((seq_t)49 * a1);
                a1 = a2;
                a2 = next;
            }
            return next;
        }
        cache = new_cache;
        capacity = new_capacity;
    }

    /* compute terms that are not yet cached */
    for (idx_t i = max_cached + 1; i <= n; ++i) {
        cache[i] = ((seq_t)8 * cache[i - 1]) + ((seq_t)49 * cache[i - 2]);
    }
    max_cached = n;
    return cache[n];
}
