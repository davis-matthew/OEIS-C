/*
 * A209075  Number of 6-bead necklaces labeled with numbers -n..n not allowing
 *          reversal, with sum zero and avoiding the pattern z z+1 z+2.
 * https://oeis.org/A209075
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*a(n-1) - 10*a(n-2) + 11*a(n-3) - 10*a(n-4) + 11*a(n-5)
 *            - 10*a(n-6) + 5*a(n-7) - a(n-8) for n > 9. Initial values
 *            (offset 1): a(1)=19, a(2)=265, a(3)=1465, a(4)=5239, a(5)=14431,
 *            a(6)=33469, a(7)=68723, a(8)=128845, a(9)=225127. The function
 *            stores computed terms in a static cache to allo...
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 67, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* realloc, abort */

static const seq_t init_vals[9] = {
    19,
    265,
    1465,
    5239,
    14431,
    33469,
    68723,
    128845,
    225127
};

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined, but keep safe */

    /* static cache: cache[i] holds a(i+1) */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;   /* number of terms stored */

    if (n > cache_size) {
        /* expand cache */
        size_t new_cap = (size_t)n;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;

        /* compute missing terms */
        for (idx_t i = cache_size + 1; i <= n; ++i) {
            if (i <= 9) {
                cache[i - 1] = init_vals[i - 1];
            } else {
                idx_t j = i - 1;   /* zero‑based index for a(i) */
                seq_t val = (5 * cache[j - 1]
                         - 10 * cache[j - 2]
                         + 11 * cache[j - 3]
                         - 10 * cache[j - 4]
                         + 11 * cache[j - 5]
                         - 10 * cache[j - 6]
                         + 5  * cache[j - 7]
                         -    cache[j - 8]);
                cache[j] = val;
            }
        }
        cache_size = n;
    }

    return cache[n - 1];
}
