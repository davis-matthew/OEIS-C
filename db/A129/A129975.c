/*
 * A129975  Nonnegative values x of solutions (x, y) to the Diophantine
 *          equation x^2+(x+953)^2 = y^2.
 * https://oeis.org/A129975
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-3)-a(n-6)+1906 for n > 6; a(1)=0, a(2)=132,
 *            a(3)=2295, a(4)=2859, a(5)=3535, a(6)=15792
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 27, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

static seq_t *cache     = NULL; /* array of computed terms, indexed by n */
static size_t cache_cap = 0;     /* allocated length of cache */
static idx_t  computed_upto = 0; /* greatest n for which cache[n] is valid */

/* initial values for n=0..6 (index 0 unused, but set to 0 for safety) */
static const seq_t init[7] = {0, 0, 132, 2295, 2859, 3535, 15792};

/* make sure cache holds terms up to n and compute any missing ones */
static void ensure_cache(idx_t n)
{
    if (n <= computed_upto) return;

    /* grow the cache if necessary */
    if ((size_t)(n + 1) > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 64;
        while (new_cap <= (size_t)n) new_cap <<= 1;

        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();           /* allocation failed */
        cache     = tmp;
        cache_cap = new_cap;
    }

    /* compute and store any missing terms */
    for (idx_t i = computed_upto + 1; i <= n; ++i) {
        if (i <= 6) {
            cache[(size_t)i] = init[(size_t)i];
        } else {
            cache[(size_t)i] = 6 * cache[(size_t)(i - 3)]
                               - cache[(size_t)(i - 6)]
                               + (seq_t)1906;
        }
    }
    computed_upto = n;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* undefined for n<1, return 0 */
    ensure_cache(n);
    return cache[(size_t)n];
}
