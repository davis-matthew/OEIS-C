/*
 * A188123  Number of strictly increasing arrangements of 4 nonzero numbers in
 *          -(n+2)..(n+2) with sum zero.
 * https://oeis.org/A188123
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1)-a(n-3)-a(n-4)+2*a(n-6)-a(n-7), a(0)=1, a(1)=3,
 *            a(2)=8, a(3)=16, a(4)=31, a(5)=51, a(6)=80
 * Range:     u8 n <= 9, u16 n <= 65, u32 n <= 200, u64 n <= 200,
 *            u128 n <= 200, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_cap = 0;

static const seq_t INIT[7] = { (seq_t)1, (seq_t)3, (seq_t)8,
                               (seq_t)16, (seq_t)31, (seq_t)51,
                               (seq_t)80 };

seq_t a(idx_t n)
{
    if (cache_cap <= n) {
        size_t new_cap = (size_t)(n + 1);
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache)
            return (seq_t)0;          /* allocation failed */

        cache = new_cache;

        for (idx_t i = cache_cap; i <= n; ++i) {
            if (i < 7) {               /* initial values */
                cache[i] = INIT[i];
            } else {                   /* recurrence */
                seq_t val = cache[i - 1];
                val = val + cache[i - 1];   /* 2*a(i-1) */
                val = val + cache[i - 6];
                val = val + cache[i - 6];   /* +2*a(i-6) */

                seq_t sub = cache[i - 3];
                sub = sub + cache[i - 4];
                sub = sub + cache[i - 7];   /* a(i-3)+a(i-4)+a(i-7) */

                cache[i] = val - sub;
            }
        }

        cache_cap = new_cap;
    }

    return cache[n];
}
