/*
 * A147676  Add 10, divide by 2, repeat.
 * https://oeis.org/A147676
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(1)=90, a(2)=100; for n>=3: if n is odd: a(n) = (a(n-2)+10)/2
 *            if n is even: a(n) = (a(n-2)+20)/2 (All integer divisions are
 *            truncating towards zero.)
 * Range:     u8 n <= 10, u16 n <= 10, u32 n <= 10, u64 n <= 10, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t computed = 0;

    if (n == 0) {               /* offset is 1, this case is unused */
        return 0;
    }

    /* Grow the cache if the requested index is beyond what we have computed */
    if ((idx_t)cache_cap < n) {
        size_t new_cap = (size_t)n;
        if (new_cap < 8) new_cap = 8;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) {             /* allocation failure – unlikely in test harness */
            return 0;
        }
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute new terms */
    for (idx_t i = computed + 1; i <= n; ++i) {
        if (i == 1) {
            cache[0] = 90;
        } else if (i == 2) {
            cache[1] = 100;
        } else if (i & 1ULL) {  /* odd n */
            cache[i - 1] = (cache[i - 3] + 10) / 2;
        } else {                /* even n */
            cache[i - 1] = (cache[i - 3] + 20) / 2;
        }
    }
    computed = n;

    return cache[n - 1];
}
