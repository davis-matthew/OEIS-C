/*
 * A133284  Indices of the 12-gonal numbers which are also centered 12-gonal
 *          number or numbers X such that 30*X^2-24*X+3 is a square.
 * https://oeis.org/A133284
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1, a(2)=13, a(n)=22*a(n-1)-a(n-2)-8 for n≥3
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 15, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

static seq_t *cache = NULL;      /* cached sequence values */
static size_t cache_cap = 0;     /* allocated capacity of the cache */
static size_t cache_len = 0;     /* number of terms that have been computed */

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;            /* undefined below the offset */

    /* initialise the cache with the first two terms */
    if (cache == NULL) {
        cache_cap = 2;
        cache = (seq_t *)realloc(NULL, cache_cap * sizeof(seq_t));
        if (cache == NULL) exit(1);        /* allocation failure */
        cache[0] = (seq_t)1;              /* a(1) */
        cache[1] = (seq_t)13;             /* a(2) */
        cache_len = 2;
    }

    /* grow the cache if necessary */
    if ((size_t)n > cache_cap) {
        size_t new_cap = cache_cap;
        while ((size_t)n > new_cap) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) exit(1);
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute any missing terms up to n */
    if ((size_t)n > cache_len) {
        for (size_t i = cache_len + 1; i <= (size_t)n; ++i) {
            /* i is 1‑based; cache index is i‑1 */
            seq_t prev  = cache[i - 2];   /* a(i‑1)   */
            seq_t prev2 = cache[i - 3];   /* a(i‑2)   */
            cache[i - 1] = (seq_t)22 * prev - prev2 - (seq_t)8;
        }
        cache_len = (size_t)n;
    }

    return cache[(size_t)n - 1];
}
