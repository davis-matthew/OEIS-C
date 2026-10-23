/*
 * A122023  Expansion of
 *          x^2*(1+3*x+x^2-x^3+28*x^4+80*x^5)/(1-10*x^2+29*x^4-24*x^6).
 * https://oeis.org/A122023
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(1)=0, a(2)=1, a(3)=3, a(4)=11, a(5)=29, a(6)=109, a(7)=283;
 *            for n≥8: a(n)=10*a(n‑2)−29*a(n‑4)+24*a(n‑6)
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 25, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* 1‑based cache for already computed terms */
static seq_t *cache = NULL;
static idx_t cache_size = 0;         /* allocated size (≥ 1)          */
static idx_t computed_up_to = 0;     /* highest index already stored */

/* Return the n‑th term of OEIS A122023 (offset 1). */
seq_t a(idx_t n)
{
    if (n < 1)                 /* invalid index – definition starts at 1 */
        return (seq_t)0;

    /* enlarge the cache if necessary */
    if (n > cache_size) {
        seq_t *newp = (seq_t *)realloc(cache, (n + 1) * sizeof *newp);
        if (!newp)            /* allocation failed */
            return (seq_t)0;
        cache = newp;
        cache_size = n;
    }

    /* compute missing terms up to n */
    for (idx_t i = computed_up_to + 1; i <= n; ++i) {
        switch (i) {
            case 1: cache[1] = (seq_t)0; break;
            case 2: cache[2] = (seq_t)1; break;
            case 3: cache[3] = (seq_t)3; break;
            case 4: cache[4] = (seq_t)11; break;
            case 5: cache[5] = (seq_t)29; break;
            case 6: cache[6] = (seq_t)109; break;
            case 7: cache[7] = (seq_t)283; break;
            default: {
                seq_t v = (seq_t)10 * cache[i - 2];
                v -= (seq_t)29 * cache[i - 4];
                v += (seq_t)24 * cache[i - 6];
                cache[i] = v;
            }
        }
    }
    computed_up_to = n;
    return cache[n];
}
