/*
 * A220864  Number of perfect matchings in the graph C_4 X C_n.
 * https://oeis.org/A220864
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 16, u64 n <= 33, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A220864: number of perfect matchings in C4 × Cn
   Recurrence (order 8):
        a(n) = 4*a(n-1) + 6*a(n-2) - 28*a(n-3)              \
               + 28*a(n-5) - 6*a(n-6) - 4*a(n-7) + a(n-8)
   Offset: a(3)=50
   Initial values: a(0)=a(1)=a(2)=0,
                   a(3)=50, a(4)=272, a(5)=722, a(6)=3108,
                   a(7)=10082, a(8)=39952, a(9)=140450,
                   a(10)=537636 */
#include <stdlib.h>
#include <string.h>

static seq_t *cache = NULL;      /* cached sequence values   */
static idx_t cache_len = 0;      /* first unused index      */
static size_t cache_cap = 0;     /* current capacity        */

/* ensure cache is allocated up to (n+1) elements */
static void ensure_cache(idx_t n)
{
    if (cache == NULL) {
        cache_cap = (n >= 32) ? 64 : 32;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        memset(cache, 0, cache_cap * sizeof(seq_t));
        cache_len = 0;
    }
    while (n + 1 > cache_cap) {
        size_t new_cap = cache_cap * 2;
        cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        memset(cache + cache_cap, 0, (new_cap - cache_cap) * sizeof(seq_t));
        cache_cap = new_cap;
    }
    if (cache_len < n + 1) {
        const seq_t known[11] = {
            0, 0, 0, 50, 272, 722, 3108, 10082,
            39952, 140450, 537636
        };
        for (idx_t i = cache_len; i <= n; ++i) {
            if (i <= 10) {
                cache[i] = known[ (size_t)i ];
            } else {
                /* recurrence: a(n) = pos - neg
                   pos  = 4*a(n-1) + 6*a(n-2) + 28*a(n-5) + a(n-8)
                   neg  = 28*a(n-3) + 6*a(n-6) + 4*a(n-7)
                */
                seq_t pos = (seq_t)4 * cache[i-1]
                          + (seq_t)6 * cache[i-2]
                          + (seq_t)28 * cache[i-5]
                          +        cache[i-8];
                seq_t neg = (seq_t)28 * cache[i-3]
                          + (seq_t)6 * cache[i-6]
                          + (seq_t)4 * cache[i-7];
                cache[i] = pos - neg;
            }
        }
        cache_len = n + 1;
    }
}

seq_t a(idx_t n)
{
    if (n < 3) return 0;   /* values below offset are irrelevant */
    ensure_cache(n);
    return cache[n];
}
