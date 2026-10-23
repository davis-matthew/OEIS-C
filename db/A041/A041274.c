/*
 * A041274  Numerators of continued fraction convergents to sqrt(150).
 * https://oeis.org/A041274
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) = 98*a(n-2) - a(n-4), a(0)=12, a(1)=49, a(2)=1188,
 *            a(3)=4801
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 18, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t */

#ifdef __cplusplus
#include <vector>
static std::vector<seq_t> a_cache;
#else
static seq_t *a_cache = NULL;
static size_t a_cache_size = 0;
#endif

seq_t a(idx_t n)
{
    static int initialized = 0;
    if (!initialized) {
#ifdef __cplusplus
        a_cache.resize(8);
#else
        a_cache_size = 8;
        a_cache = (seq_t*)calloc(a_cache_size, sizeof(seq_t));
#endif
        const seq_t base[8] = {12, 49, 1188, 4801,
                               116412, 470449, 11407188, 46099201};
        for (size_t i = 0; i < 8; ++i) {
#ifdef __cplusplus
            a_cache[i] = base[i];
#else
            a_cache[i] = base[i];
#endif
        }
        initialized = 1;
    }

    if (n < 8) {
#ifdef __cplusplus
        return a_cache[n];
#else
        return a_cache[n];
#endif
    }

    /* Compute up to n if needed */
    for (idx_t i = 8; i <= n; ++i) {
#ifdef __cplusplus
        if (a_cache.size() <= (size_t)i)
            a_cache.resize(i + 1);
        a_cache[i] = 98 * a_cache[i - 2] - a_cache[i - 4];
#else
        if (a_cache_size <= (size_t)i) {
            size_t new_cap = a_cache_size * 2;
            if (new_cap == 0) new_cap = 8;
            while (new_cap <= (size_t)i) new_cap *= 2;
            seq_t *tmp = (seq_t*)realloc(a_cache, new_cap * sizeof(seq_t));
            a_cache = tmp;
            a_cache_size = new_cap;
        }
        a_cache[i] = 98 * a_cache[i - 2] - a_cache[i - 4];
#endif
    }

#ifdef __cplusplus
    return a_cache[n];
#else
    return a_cache[n];
#endif
}
