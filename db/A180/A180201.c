/*
 * A180201  Inverse permutation to A180200.
 * https://oeis.org/A180201
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=0, a(1)=1, a(2*n) = 2*a(n) + [n even], a(2*n+1) = 2*a(n) +
 *            [n odd] (for n>0)
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of computed values */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;        /* number of allocated entries */

    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (cache == NULL) ?
                     (seq_t*)malloc(new_cap * sizeof(seq_t)) :
                     (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) {
            /* allocation failed – return 0 as a fall‑back */
            return 0;
        }

        /* initialise first entries if this is the first allocation */
        if (cache_cap == 0) {
            tmp[0] = 0;
            if (new_cap > 1)
                tmp[1] = 1;
        }
        /* if we are just extending beyond a single element,
           ensure a(1) is present (realloc may lose it). */
        else if (cache_cap == 1 && new_cap > 1) {
            tmp[1] = 1;
        }

        /* compute new entries using the recurrence */
        size_t start = (cache_cap >= 2) ? cache_cap : 2;
        for (size_t i = start; i < new_cap; ++i) {
            size_t k = i >> 1;          /* floor(i/2) */
            seq_t val = tmp[k] << 1;    /* 2 * a(k) */

            if ((i & 1) == 0) {         /* i is even: a(2*k) */
                if ((k & 1) == 0)       /* k even */
                    val += 1;
            } else {                    /* i is odd: a(2*k+1) */
                if ((k & 1) == 1)       /* k odd */
                    val += 1;
            }
            tmp[i] = val;
        }

        cache = tmp;
        cache_cap = new_cap;
    }

    return cache[(size_t)n];
}
