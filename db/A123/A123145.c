/*
 * A123145  a(1) = 1, a(n) = a(n-1) if n == 1 (mod 4), otherwise a(n) = n *
 *          a(n-1) for n >= 2.
 * https://oeis.org/A123145
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1; a(n)=a(n-1) if n mod 4 == 1, else a(n)=n*a(n-1) for n>=2
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 15, u64 n <= 25, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, realloc, free, exit */

seq_t a(idx_t n) {
    /* static cache: cache[i] holds a(i) for i>=1, cache[0] is unused */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;   /* highest index stored (0 if none) */

    if (n == 0)   /* undefined, but return 0 to avoid UB */
        return (seq_t)0;

    /* Grow cache to cover n */
    if (n > cache_size) {
        idx_t new_size = n + 1;          /* +1 because we index from 1 */
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) exit(1);               /* allocation failure */
        cache = tmp;

        /* Initialize base case if cached array was empty */
        if (cache_size == 0) {
            cache[1] = (seq_t)1;          /* a(1)=1 */
            cache_size = 1;
        }

        /* Compute values up to n */
        for (idx_t i = cache_size + 1; i <= n; ++i) {
            if (i % 4 == 1) {
                cache[i] = cache[i - 1];
            } else {
                cache[i] = cache[i - 1] * (seq_t)i;
            }
        }
        cache_size = n;
    }

    return cache[n];
}
