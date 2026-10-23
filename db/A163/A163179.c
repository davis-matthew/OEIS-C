/*
 * A163179  Number of reduced words of length n in Coxeter group on 27
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^4 = I.
 * https://oeis.org/A163179
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 25*a(n‑1) + 25*a(n‑2) + 25*a(n‑3) – 325*a(n‑4) for n ≥ 5
 *            Initial values: a(0)=1, a(1)=27, a(2)=702, a(3)=18252,
 *            a(4)=474201
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, realloc, free, exit */

static const seq_t init_vals[5] = { (seq_t)1, (seq_t)27, (seq_t)702,
                                    (seq_t)18252, (seq_t)474201 };

seq_t a(idx_t n)
{
    /* static cache of computed terms */
    static seq_t *cache   = NULL;
    static size_t cache_size = 0;   /* number of terms stored (highest index + 1) */
    static size_t cache_cap  = 0;   /* allocated array capacity */

    /* allocate initial cache if needed */
    if (!cache) {
        cache_cap = 16;                     /* start with a reasonable size */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) exit(EXIT_FAILURE);
    }

    /* enlarge cache capacity if required */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = cache_cap;
        if (new_cap < 16) new_cap = 16;
        while (new_cap < (size_t)n + 1) new_cap <<= 1;   /* double until enough */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute missing terms up to n */
    for (size_t i = cache_size; i <= (size_t)n; ++i) {
        if (i < 5) {                         /* use the initial values */
            cache[i] = init_vals[i];
        } else {                             /* recurrence for n ≥ 5 */
            cache[i] = cache[i-1] * 25
                      + cache[i-2] * 25
                      + cache[i-3] * 25
                      - cache[i-4] * 325;
        }
    }

    cache_size = (size_t)n + 1;
    return cache[n];
}
