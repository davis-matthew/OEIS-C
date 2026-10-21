/*
 * A063227  Dimension of the space of weight 2n cuspidal newforms for Gamma_0(
 *          66 ).
 * https://oeis.org/A063227
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-2)+a(n-3)-a(n-5) for n>6, with initial values a(1)=3,
 *            a(2)=4, a(3)=8, a(4)=12, a(5)=16, a(6)=16
 * Range:     u8 n <= 50, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h> /* for size_t */
#include <stdlib.h> /* for malloc, realloc */

static const seq_t init[6] = {3, 4, 8, 12, 16, 16};

static seq_t *cache = NULL;      /* cache[1..max_n] */
static size_t cache_cap = 0;     /* current allocated size (indices 0..cache_cap-1) */

seq_t a(idx_t n) {
    if (n < 1) return 0; /* undefined for offset < 1 */

    /* Ensure cache is large enough for index n */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;          /* allocation failure: return 0 as fallback */
        cache = tmp;
        /* Initialize newly allocated slots to 0 */
        for (size_t i = cache_cap; i < new_cap; ++i) cache[i] = 0;
        cache_cap = new_cap;
    }

    /* Compute missing entries up to n */
    for (size_t i = 1; i <= (size_t)n; ++i) {
        if (i <= 6) {
            cache[i] = init[i - 1];
        } else if (cache[i] == 0) {
            cache[i] = cache[i - 2] + cache[i - 3] - cache[i - 5];
        }
    }

    return cache[(size_t)n];
}
