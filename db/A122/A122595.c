/*
 * A122595  Expansion of x/(1 - 3*x + x^2 + x^3 - x^4).
 * https://oeis.org/A122595
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) - a(n-2) - a(n-3) + a(n-4); initialized with
 *            a(1)=1, a(2)=3, a(3)=8, a(4)=20
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 24, u64 n <= 49, u128 n <= 97,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

#ifdef __cplusplus
#include <vector>
static std::vector<seq_t> cache(1);          /* index 0 unused   */
static idx_t current_max_n = 0;              /* highest n cached */
#else
static seq_t *cache = NULL;                  /* index 0 unused   */
static idx_t current_max_n = 0;              /* highest n cached */
#endif

seq_t a(idx_t n)
{
    if (n < 1u)                     /* below offset, behaviour undefined */
        return (seq_t)0;

    /* enlarge cache if necessary */
    if (n > current_max_n) {
#ifdef __cplusplus
        /* ensure vector has enough space */
        if (cache.size() <= n)
            cache.resize(n + 1);
#else
        seq_t *new_cache = realloc(cache, (size_t)(n + 1) * sizeof(seq_t));
        if (!new_cache)           /* allocation failure */
            return (seq_t)0;
        cache = new_cache;
#endif
        /* compute missing terms */
        for (idx_t i = current_max_n + 1; i <= n; ++i) {
            if (i == 1u)         cache[i] = (seq_t)1;
            else if (i == 2u)    cache[i] = (seq_t)3;
            else if (i == 3u)    cache[i] = (seq_t)8;
            else if (i == 4u)    cache[i] = (seq_t)20;
            else
                cache[i] = (seq_t)3 * cache[i-1] - cache[i-2] - cache[i-3] + cache[i-4];
        }
        current_max_n = n;
    }

    return cache[n];
}
