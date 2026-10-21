/*
 * A229448  Number of 5 X n 0..2 arrays with horizontal differences mod 3
 *          never 1, vertical differences mod 3 never -1, and rows and columns
 *          lexicographically nondecreasing.
 * https://oeis.org/A229448
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*a(n-1) - 15*a(n-2) + 20*a(n-3) - 15*a(n-4) + 6*a(n-5) -
 *            a(n-6) * for n > 6, * with initial values a(1)=17, a(2)=47,
 *            a(3)=128, a(4)=324, a(5)=753, a(6)=1609
 * Range:     u8 n <= 3, u16 n <= 13, u32 n <= 119, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/mul_small.h"   /* provides seq_t mul_small(seq_t, unsigned int) */

seq_t a(idx_t n) {
    /* cache of previously computed values; 1‑based indexing */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;   /* maximum n that has been computed so far */

    if (n < 1) return (seq_t)0;   /* offset starts at 1 */

    /* grow cache if necessary */
    if (n > cache_len) {
        size_t new_size = (size_t)(n + 1);             /* allocate for indices 0..n */
        seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!new_cache) return (seq_t)0;               /* out of memory, return 0 */
        cache = new_cache;

        /* compute new entries */
        for (idx_t i = cache_len + 1; i <= n; ++i) {
            if (i == 1)          cache[i] = (seq_t)17;
            else if (i == 2)     cache[i] = (seq_t)47;
            else if (i == 3)     cache[i] = (seq_t)128;
            else if (i == 4)     cache[i] = (seq_t)324;
            else if (i == 5)     cache[i] = (seq_t)753;
            else if (i == 6)     cache[i] = (seq_t)1609;
            else {
                /* recurrence: a(i) = 6*a(i-1) - 15*a(i-2) + 20*a(i-3)
                 *               - 15*a(i-4) + 6*a(i-5) - a(i-6) */
                seq_t val = mul_small(cache[i-1], 6);
                val -= mul_small(cache[i-2], 15);
                val += mul_small(cache[i-3], 20);
                val -= mul_small(cache[i-4], 15);
                val += mul_small(cache[i-5], 6);
                val -= cache[i-6];
                cache[i] = val;
            }
        }
        cache_len = n;
    }
    return cache[n];
}
