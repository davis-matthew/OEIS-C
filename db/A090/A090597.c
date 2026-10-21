/*
 * A090597  a(n) = - a(n-1) + 5(a(n-2) + a(n-3)) - 2(a(n-4) + a(n-5)) -
 *          8(a(n-6) + a(n-7)).
 * https://oeis.org/A090597
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = a(n-1)+3*a(n-2)-a(n-3)-2*a(n-5)-4*a(n-6)
 * Range:     u8 n <= 13, u16 n <= 21, u32 n <= 37, u64 n <= 69, u128 n <= 133,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* definition for n ≥ 3; base values: a(3)=0, a(4)=1, a(5)=1, a(6)=3, a(7)=3, a(8)=8 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* cache of previously computed terms: static across calls */
    static seq_t *cache    = NULL;
    static size_t cache_size = 0;        /* number of allocated slots        */
    static idx_t  computed   = 0;        /* next index that needs computing */

    if (n < 3)                 /* sequence undefined below offset */
        return 0;

    /* extend cache if necessary */
    if (cache_size <= (size_t)n) {
        cache_size = (size_t)n + 1;
        cache = (seq_t*)realloc(cache, cache_size * sizeof(seq_t));
    }

    /* compute missing terms up to n (in increasing order) */
    for (idx_t i = computed; i <= n; ++i) {
        if (i < 3) {
            cache[i] = 0;
        } else if (i == 3) {
            cache[i] = 0;
        } else if (i == 4) {
            cache[i] = 1;
        } else if (i == 5) {
            cache[i] = 1;
        } else if (i == 6) {
            cache[i] = 3;
        } else if (i == 7) {
            cache[i] = 3;
        } else if (i == 8) {
            cache[i] = 8;
        } else { /* i >= 9: recurrence of order 6 */
            cache[i] =
                cache[i-1]
                + (seq_t)3 * cache[i-2]
                - cache[i-3]
                - (seq_t)2 * cache[i-5]
                - (seq_t)4 * cache[i-6];
        }
    }
    computed = n + 1;          /* next index to compute in future calls */
    return cache[n];
}
