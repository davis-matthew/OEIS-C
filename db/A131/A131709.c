/*
 * A131709  Number of partitions into "bus routes" of an n X 1 grid.
 * https://oeis.org/A131709
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=4, a(2)=14, for n>=3: a(n) = 10*(a(n-1)-a(n-2)) + 4
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 21, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#ifdef __cplusplus
#include <vector>
#else
#include <stdlib.h>
#endif

seq_t a(idx_t n)
{
#ifdef __cplusplus
    static std::vector<seq_t> cache;
    static idx_t computed_up_to = 0;
    /* Ensure capacity for index n */
    if (cache.empty()) {
        cache.resize(3);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)4;
        cache[2] = (seq_t)14;
        computed_up_to = 2;
    }
    if (n + 1 > cache.size())
        cache.resize(n + 1);

    if (n <= 2)          /* already initialised */
        return cache[n];

    if (computed_up_to < n) {
        const seq_t TEN = (seq_t)10;
        const seq_t FOUR = (seq_t)4;
        for (idx_t i = computed_up_to + 1; i <= n; ++i)
            cache[i] = TEN * (cache[i - 1] - cache[i - 2]) + FOUR;
        computed_up_to = n;
    }
    return cache[n];
#else
    static seq_t *cache = NULL;
    static size_t cap = 0;
    static idx_t computed_up_to = 0;

    /* Allocate initial array if needed */
    if (cache == NULL) {
        cap = 3;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)4;
        cache[2] = (seq_t)14;
        computed_up_to = 2;
    }

    /* Increase capacity if required */
    if ((size_t)(n + 1) > cap) {
        size_t new_cap = cap;
        while ((size_t)(n + 1) > new_cap) {
            new_cap = (new_cap > 0) ? new_cap * 2 : 8;
        }
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cap = new_cap;
    }

    if (n <= 2)
        return cache[n];

    if (computed_up_to < n) {
        const seq_t TEN = (seq_t)10;
        const seq_t FOUR = (seq_t)4;
        for (idx_t i = computed_up_to + 1; i <= n; ++i)
            cache[i] = TEN * (cache[i - 1] - cache[i - 2]) + FOUR;
        computed_up_to = n;
    }
    return cache[n];
#endif
}
