/*
 * A041285  Denominators of continued fraction convergents to sqrt(155).
 * https://oeis.org/A041285
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 498*a(n-4) - a(n-8); Initial values: a(0)=1, a(1)=2,
 *            a(2)=9, a(3)=20, a(4)=489, a(5)=998, a(6)=4481, a(7)=9960
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 29, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* base values for n = 0 .. 7 */
    static const seq_t base[8] = {
        (seq_t)1, (seq_t)2, (seq_t)9, (seq_t)20,
        (seq_t)489, (seq_t)998, (seq_t)4481, (seq_t)9960
    };

    /* trivial case: n < 8 */
    if (n < 8) {
        return base[n];
    }

    /* memoisation cache */
    static seq_t *cache = NULL;
    static size_t capacity = 0;

    /* initialise cache on first use */
    if (cache == NULL) {
        capacity = 8;
        cache   = (seq_t*)malloc(capacity * sizeof(seq_t));
        if (cache == NULL) {
            return (seq_t)0;           /* out of memory */
        }
        for (idx_t i = 0; i < 8; ++i) {
            cache[i] = base[i];
        }
    }

    /* grow cache if necessary */
    size_t required = (size_t)(n) + 1;
    if (required > capacity) {
        size_t new_cap = capacity ? capacity * 2 : 16;
        while (new_cap <= required) {
            new_cap *= 2;
        }
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) {
            return (seq_t)0;           /* out of memory */
        }
        cache   = tmp;
        capacity = new_cap;
    }

    /* compute missing values up to n */
    for (idx_t i = 8; i <= n; ++i) {
        seq_t t = cache[i - 4];
        t *= (seq_t)498;
        t -= cache[i - 8];
        cache[i] = t;
    }

    return cache[n];
}
