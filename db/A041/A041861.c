/*
 * A041861  Denominators of continued fraction convergents to sqrt(452).
 * https://oeis.org/A041861
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(0)=1, a(1)=3, a(2)=4, a(3)=23, a(4)=73, a(5)=753, a(6)=2332,
 *            a(7)=12413, a(8)=14745, a(9)=56648, a(10)=2393961,
 *            a(11)=7238531, a(12)=9632492, a(13)=55400991, a(14)=175835465,
 *            a(15)=1813755641, a(16)=5617102388, a(17)=29899267581,
 *            a(18)=35516369969, a(19)=136448377488, recurrence: a(n) =
 *            2408706...
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 15, u64 n <= 30, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#ifndef _SEQ_A041861_H_
#define _SEQ_A041861_H_

#include <stdlib.h>
#include <stdint.h>

/* Return the n-th term of OEIS A041861.  */
seq_t a(idx_t n)
{
    /* Initial 20 terms (a(0)…a(19)).  */
    static const seq_t init[20] = {
        1ull, 3ull, 4ull, 23ull, 73ull, 753ull, 2332ull, 12413ull,
        14745ull, 56648ull, 2393961ull, 7238531ull, 9632492ull,
        55400991ull, 175835465ull, 1813755641ull,
        5617102388ull, 29899267581ull, 35516369969ull,
        136448377488ull
    };

    /* Static cache for computed values.  */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;     /* Number of allocated elements.  */
    static idx_t computed   = 19;     /* Highest index that has been computed.  */

    /* Allocate and initialise the cache on first call.  */
    if (cache == NULL) {
        cache_size = (idx_t)20;          /* Allocate space for a(0)…a(19).  */
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (cache == NULL) return 0;     /* Allocation failed; return 0.  */
        for (idx_t i = 0; i < cache_size; ++i) {
            cache[i] = init[i];
        }
    }

    /* Expand cache if necessary.  */
    if (n + 1 > cache_size) {
        idx_t new_size = n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (new_cache == NULL) return 0;      /* Allocation failed.  */
        cache = new_cache;
        cache_size = new_size;
    }

    /* Compute missing terms using the recurrence.  */
    if (n > computed) {
        for (idx_t i = computed + 1; i <= n; ++i) {
            /* a(i) = 2408706 * a(i-10) - a(i-20)  */
            seq_t term = cache[i - 10];
            term *= (seq_t)2408706;
            term -= cache[i - 20];
            cache[i] = term;
        }
        computed = n;
    }

    return cache[n];
}

#endif /* _SEQ_A041861_H_ */
