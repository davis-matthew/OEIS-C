/*
 * A074524  a(n) = 1^n + 7^n + 9^n.
 * https://oeis.org/A074524
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = 1^n + 7^n + 9^n
 * Recurrence: a(0) = 3, a(1) = 17,
 *              a(n) = 16*a(n-1) - 63*a(n-2) + 48   for n ≥ 2
 */
#include <stdlib.h>
#include <stdint.h>

/* External linkage required */
seq_t a(idx_t n)
{
    /* static cache of previously computed values */
    static seq_t *cache = NULL;
    static idx_t cached = 0;      /* number of initialized elements */
    static idx_t capacity = 0;     /* allocated capacity */

    /* initialise cache on first call */
    if (cache == NULL) {
        capacity = 2;
        cache = (seq_t *)malloc(capacity * sizeof(seq_t));
        if (!cache) return 0;           /* allocation failure, return 0 */
        cache[0] = (seq_t)3;
        cache[1] = (seq_t)17;
        cached = 2;
    }

    /* grow the cache if necessary */
    if (n >= capacity) {
        idx_t newcap = capacity ? capacity * 2 : 2;
        while (n >= newcap) newcap <<= 1;     /* double until enough */
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) return 0;                      /* allocation failure */
        cache = tmp;
        capacity = newcap;
    }

    /* compute missing terms */
    if (cached <= n) {
        idx_t i;
        for (i = cached; i <= n; ++i) {
            seq_t t1 = cache[i-1];
            t1 *= (seq_t)16;
            seq_t t2 = cache[i-2];
            t2 *= (seq_t)63;
            cache[i] = t1 - t2 + (seq_t)48;
        }
        cached = n + 1;
    }

    return cache[n];
}
