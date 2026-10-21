/*
 * A051253  Weights of rotation-symmetric functions in n variables.
 * https://oeis.org/A051253
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nice, easy, nonn
 * Formula:   a(n) = 2*a(n-1)+2*a(n-2)-2*a(n-3)-4*a(n-4) for n >= 7; base
 *            values: a(3)=1, a(4)=4, a(5)=6, a(6)=18
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 33, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>


seq_t a(idx_t n)
{
    if (n < 3) return 0;           /* undefined for indices below the offset */

    static seq_t *cache = NULL;    /* cache[0] == a(3) */
    static idx_t cache_len = 0;    /* number of stored terms */
    static idx_t cache_cap = 0;    /* allocated capacity */

    /* Initialize base terms on first call */
    if (cache_len == 0) {
        cache_cap = 8;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0;      /* allocation failure */
        cache[0] = (seq_t)1;       /* a(3) */
        cache[1] = (seq_t)4;       /* a(4) */
        cache[2] = (seq_t)6;       /* a(5) */
        cache[3] = (seq_t)18;      /* a(6) */
        cache_len = 4;
    }

    /* Return cached value if already computed */
    if (n < 3 + cache_len) {
        return cache[n - 3];
    }

    /* Compute successive terms up to n */
    for (idx_t k = 3 + cache_len; k <= n; ++k) {
        /* Ensure capacity for the new term */
        if (cache_len >= cache_cap) {
            idx_t new_cap = cache_cap * 2;
            if (new_cap == 0) new_cap = 8;
            while (new_cap <= cache_len) new_cap <<= 1;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) return 0;      /* allocation failure */
            cache = tmp;
            cache_cap = new_cap;
        }

        seq_t t1 = cache[(k - 1) - 3];
        seq_t t2 = cache[(k - 2) - 3];
        seq_t t3 = cache[(k - 3) - 3];
        seq_t t4 = cache[(k - 4) - 3];

        seq_t val = 2 * t1 + 2 * t2 - 2 * t3 - 4 * t4;
        cache[cache_len++] = val;
    }

    return cache[n - 3];
}
