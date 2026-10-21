/*
 * A218222  G.f. A(x) satisfies: A(x) = x + x*[d/dx A(x)^2].
 * https://oeis.org/A218222
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n * sum_{i=1}^{n-1} a(i) * a(n-i), a(1)=1
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

#ifdef __cplusplus
#include <vector>
#endif

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /* undefined, but harmless */

#ifdef __cplusplus
    static std::vector<seq_t> cache;     /* indices start at 1 */
    static idx_t computed = 0;
#else
    static seq_t *cache = NULL;          /* dynamic array */
    static size_t cap = 0;
    static idx_t computed = 0;
#endif

    /* lazy initialization */
#ifdef __cplusplus
    if (cache.empty()) {
        cache.resize(2);                 /* indices 0,1 */
        cache[1] = 1;                    /* a(1) = 1 */
        computed = 1;
    }
#else
    if (cache == NULL) {
        size_t init = 16;
        cache = (seq_t *)calloc(init, sizeof(seq_t));
        if (!cache) return (seq_t)0;    /* allocation failure */
        cap = init;
        cache[1] = 1;                    /* a(1) = 1 */
        computed = 1;
    }
#endif

    /* extend storage if necessary */
#ifdef __cplusplus
    if ((size_t)(n + 1) > cache.size()) {
        cache.resize(n + 1);
    }
#else
    if ((size_t)(n + 1) > cap) {
        size_t new_cap = cap ? cap : 16;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;      /* allocation failure */
        cache = tmp;
        cap = new_cap;
    }
#endif

    /* compute missing terms up to n */
    for (idx_t k = computed + 1; k <= n; ++k) {
        seq_t sum = 0;
        idx_t half = (k - 1) / 2;

        /* pairs (i, k-i) with i < k-i */
        for (idx_t i = 1; i <= half; ++i) {
            seq_t prod = cache[(size_t)i] * cache[(size_t)(k - i)];
            sum += prod;
            sum += prod;                  /* double the product */
        }

        /* middle term when k is even */
        if (k % 2 == 0) {
            seq_t mid = cache[(size_t)(k / 2)];
            sum += mid * mid;
        }

        cache[(size_t)k] = (seq_t)k * sum;
    }
    computed = n;

    return cache[(size_t)n];
}
