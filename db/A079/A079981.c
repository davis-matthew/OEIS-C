/*
 * A079981  Number of permutations satisfying -k<=p(i)-i<=r and p(i)-i not in
 *          I, i=1..n, with k=3, r=3, I={-2,0,1,2}.
 * https://oeis.org/A079981
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) =
 *            a(n-4)+4*a(n-6)+2*a(n-8)+2*a(n-10)-2*a(n-12)+a(n-14)+a(n-18)
 *            with a(0)=1, a(1)=0, a(2)=0, a(3)=0, a(4)=1, a(5)=0, a(6)=2,
 *            a(7)=0, a(8)=3, a(9)=0, a(10)=8, a(11)=0, a(12)=12, a(13)=0
 * Range:     u8 n <= 21, u16 n <= 37, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache holding computed values */
    static seq_t *cache = NULL;          /* pointer to array */
    static idx_t cache_capacity = 0;     /* total allocated slots */
    static idx_t cache_init = 0;         /* number of initialized entries */

    /* base values for 0..13 */
    static const seq_t base_vals[14] = {
        1, 0, 0, 0, 1, 0, 2, 0, 3, 0, 8, 0, 12, 0
    };

    /* trivial case */
    if (n < 14) {
        return base_vals[n];
    }

    /* initialise cache with base values if needed */
    if (cache_init < 14) {
        idx_t needed = n + 1;
        cache = (seq_t *)realloc(cache, needed * sizeof(seq_t));
        cache_capacity = needed;
        for (idx_t i = 0; i < 14; ++i) {
            cache[i] = base_vals[i];
        }
        cache_init = 14;
    }

    /* extend capacity if required */
    if (cache_capacity <= n) {
        idx_t new_cap = cache_capacity * 2;
        if (new_cap <= n) new_cap = n + 1;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_capacity = new_cap;
    }

    /* compute missing terms up to n */
    if (n >= cache_init) {
        for (idx_t i = cache_init; i <= n; ++i) {
            /* recurrence computation */
            seq_t val = cache[i - 4];
            val += cache[i - 6] * 4;
            val += cache[i - 8] * 2;
            val += cache[i - 10] * 2;
            val -= cache[i - 12] * 2;
            val += cache[i - 14];
            if (i >= 18) {
                val += cache[i - 18];
            }
            cache[i] = val;
        }
        cache_init = n + 1;
    }

    return cache[n];
}
