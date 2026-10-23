/*
 * A046701  a(1) = a(2) = a(3) = 1, a(n) = a(n - a(n-1)) + a(n-1 - a(n-3)) (n
 *          > 3).
 * https://oeis.org/A046701
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 for n = 1,2,3 a(n) = a(n - a(n-1)) + a(n-1 - a(n-3))
 *            for n > 3 (OEIS A046701)
 * Range:     u8 n <= 263, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *cache = NULL;          /* array of computed terms (1‑based)   */
static size_t cache_cap = 0;         /* allocated size of cache            */
static idx_t computed_up_to = 0;     /* highest index fully computed       */

/* ensure cache has storage for indices up to n (inclusive)   */
static void _ensure_cache(idx_t n)
{
    if (cache == NULL) {
        cache_cap = n + 1;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) exit(EXIT_FAILURE);
        for (size_t i = 0; i < cache_cap; ++i) cache[i] = 0;
        cache[1] = cache[2] = cache[3] = (seq_t)1;
        computed_up_to = 3;
    } else if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = cache_cap * 2;
        if (new_cap < (size_t)n + 1) new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) exit(EXIT_FAILURE);
        cache = tmp;
        for (size_t i = cache_cap; i < new_cap; ++i) cache[i] = 0;
        cache_cap = new_cap;
    }
}

/* compute a(n) for any n >= 1   */
seq_t a(idx_t n)
{
    /* handle invalid small indices (not used in OEIS)   */
    if (n == 0) return 0;

    if (n <= 3) return (seq_t)1;

    _ensure_cache(n);

    if (n > computed_up_to) {
        for (idx_t i = computed_up_to + 1; i <= n; ++i) {
            idx_t sub1 = (idx_t)cache[i - 1];
            idx_t idx1 = i - sub1;

            idx_t sub2 = (idx_t)cache[i - 3];
            idx_t idx2 = (i - 1) - sub2;

            cache[i] = cache[idx1] + cache[idx2];
        }
        computed_up_to = n;
    }

    return cache[n];
}
