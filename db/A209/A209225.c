/*
 * A209225  Number of 4 X n 0..1 arrays avoiding 0 0 0 and 0 1 0 horizontally
 *          and 0 1 0 and 1 1 1 vertically.
 * https://oeis.org/A209225
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 21, u64 n <= 45, u128 n <= 92,
 *            big any n
 * Helpers:   seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = a(n-1) + 4*a(n-3) + 16*a(n-4) for n>7
   Initial values: a(1)=9, a(2)=81, a(3)=126, a(4)=196,
   a(5)=504, a(6)=1296, a(7)=3312 */
#include <stdlib.h>
#include "../utils/seq_realloc.h"          /* provides seq_realloc */

static seq_t *cache = NULL;                  /* cache[0] holds a(1) */
static size_t cache_len = 0, cache_cap = 0;   /* number of filled entries,
                                                 current capacity */
static const seq_t init[7] = {
    (seq_t)9,    (seq_t)81,   (seq_t)126,
    (seq_t)196,  (seq_t)504,  (seq_t)1296,
    (seq_t)3312
};

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for n<1 – not used */

    /* Grow cache if necessary */
    if (n > cache_cap) {
        seq_t *tmp = seq_realloc(cache, cache_cap, (size_t)n);
        if (!tmp) abort();          /* allocation failure */
        cache   = tmp;
        cache_cap = (size_t)n;
    }

    /* Fill missing entries */
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        if (i <= 7) {
            cache[i-1] = init[i-1];
        } else {
            cache[i-1] = cache[i-2]
                       + ( (seq_t)4  * cache[i-4] )
                       + ( (seq_t)16 * cache[i-5] );
        }
    }
    cache_len = (size_t)n;
    return cache[n-1];
}
