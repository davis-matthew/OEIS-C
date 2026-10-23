/*
 * A145131  Expansion of x/((1 - x - x^4)*(1 - x)^2).
 * https://oeis.org/A145131
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 29, u32 n <= 63, u64 n <= 132,
 *            u128 n <= 270, big any n
 * Helpers:   alloc_seq, free_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A145131: a(n) = a(n-1)+a(n-4)+n,  a(0)=0, a(1)=1, a(2)=3, a(3)=6, a(4)=10, a(5)=16 */

#include <stdlib.h>
#include "../utils/alloc_seq.h"
#include "../utils/free_seq.h"

static seq_t *cache = NULL;
static idx_t cached_max = (idx_t)-1; /* sentinel: cache not yet built */

static void compute_up_to(idx_t n)
{
    if (cache != NULL && cached_max >= n) return;   /* already have up to n */

    if (cache == NULL) {
        /* allocate at least space for six elements */
        size_t cap = (size_t)(n < 5 ? 6 : n + 1);
        cache = alloc_seq(cap);
        if (!cache) exit(EXIT_FAILURE);

        /* initialise the first six terms */
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)3;
        cache[3] = (seq_t)6;
        cache[4] = (seq_t)10;
        cache[5] = (seq_t)16;

        cached_max = 5;

        /* compute any remaining required terms */
        if (n > cached_max) {
            for (idx_t i = cached_max + 1; i <= n; ++i) {
                cache[i] = cache[i - 1] + cache[i - 4] + (seq_t)i;
            }
            cached_max = n;
        }
        return;
    }

    /* grow cache to accommodate up to n */
    size_t new_cap = (size_t)(n + 1);
    seq_t *new_cache = alloc_seq(new_cap);
    if (!new_cache) exit(EXIT_FAILURE);

    /* copy existing values */
    for (idx_t i = 0; i <= cached_max; ++i) new_cache[i] = cache[i];

    /* compute new values */
    for (idx_t i = cached_max + 1; i <= n; ++i) {
        new_cache[i] = new_cache[i - 1] + new_cache[i - 4] + (seq_t)i;
    }

    free_seq(cache);
    cache = new_cache;
    cached_max = n;
}

seq_t a(idx_t n)
{
    if (n <= 5) {
        static const seq_t init[6] = {
            (seq_t)0, (seq_t)1, (seq_t)3, (seq_t)6, (seq_t)10, (seq_t)16
        };
        return init[n];
    }
    compute_up_to(n);
    return cache[n];
}
