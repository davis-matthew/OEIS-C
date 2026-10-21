/*
 * A050352  Number of 4-level labeled linear rooted trees with n leaves.
 * https://oeis.org/A050352
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + 3 * sum_{k=1}^{n-1} binomial(n, k) * a(k) (a(0)=1)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* seq_t and idx_t are defined by the build system. */
static seq_t *cache = NULL;          /* cached values a(0)..a(max_n)   */
static idx_t cache_max = 0;          /* highest index cached */

seq_t a(idx_t n)
{
    /* Lazy initialization of cache for n=0. */
    if (cache == NULL) {
        if ((cache = (seq_t *)malloc(sizeof(seq_t))) == NULL) return 0;
        cache[0] = 1;
        cache_max = 0;
    }

    /* Expand cache if needed. */
    if (n > cache_max) {
        size_t new_size = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (tmp == NULL) return 0;  /* allocation failed */
        cache = tmp;

        /* Compute missing terms from cache_max+1 up to n. */
        for (idx_t m = cache_max + 1; m <= n; ++m) {
            seq_t sum = 0;
            seq_t C   = 1;  /* will hold binomial(m, k) for current k */

            /* k runs from 1 to m-1 */
            for (idx_t k = 1; k <= m - 1; ++k) {
                /* C = C * (m - k + 1) / k   (exact division) */
                C = C * (m - k + 1);
                C = C / k;
                sum = sum + C * cache[k];
            }

            cache[m] = 1 + 3 * sum;
        }

        cache_max = n;
    }

    return cache[n];
}
