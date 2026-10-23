/*
 * A127843  a(1) = 1, a(2) = ... = a(9) = 0, a(n) = a(n-9)+a(n-8) for n>9.
 * https://oeis.org/A127843
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=1, a(2)=...=a(9)=0, a(n)=a(n-9)+a(n-8) for n>9
 * Range:     u8 n <= 101, u16 n <= 169, u32 n <= 306, u64 n <= 579,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for memoised values */
    static seq_t *cache = NULL;      /* cache[1] .. cache[cap] */
    static idx_t cap = 0;            /* highest index allocated */
    static idx_t computed = 0;       /* highest index computed */

    if (n == 0)
        return 0;                    /* undefined, but return 0 */

    /* Allocate or grow cache array if needed */
    if (n > cap) {
        idx_t new_cap = n;
        seq_t *new_cache = (seq_t *)realloc(cache, (new_cap + 1) * sizeof(seq_t));
        if (!new_cache)
            return 0;                /* allocation failure: return 0 */
        cache = new_cache;
        cap = new_cap;
    }

    /* Compute missing values up to n */
    for (idx_t i = computed + 1; i <= n; ++i) {
        if (i == 1) {
            cache[i] = (seq_t)1;
        } else if (i <= 9) {
            cache[i] = (seq_t)0;
        } else {
            cache[i] = cache[i - 9] + cache[i - 8];
        }
    }
    computed = n;

    return cache[n];
}
