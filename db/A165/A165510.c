/*
 * A165510  a(0)=1, a(1)=9, a(n) = 72*a(n-2) - a(n-1).
 * https://oeis.org/A165510
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(0) = 1, a(1) = 9, a(n) = 72*a(n-2) - a(n-1)
 * Range:     i8 n <= 2, i16 n <= 4, i32 n <= 10, i64 n <= 20, i128 n <= 41,
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
#ifdef __cplusplus
    /* C++ implementation using std::vector for dynamic resizing */
    static std::vector<seq_t> cache;
    if (cache.empty()) {
        cache.resize(2);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)9;
    }
    if ((size_t)n >= cache.size()) {
        const size_t start = cache.size();
        cache.resize((size_t)n + 1);
        for (size_t i = start; i <= (size_t)n; ++i) {
            seq_t a_im2 = cache[i - 2];
            seq_t a_im1 = cache[i - 1];
            cache[i] = (seq_t)72 * a_im2 - a_im1;
        }
    }
    return cache[(size_t)n];
#else
    /* C implementation using malloc/realloc */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_size = 0;

    if (cache == NULL) {
        cache_cap = 2;
        cache_size = 2;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) return 0;   /* allocation failed */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)9;
    }

    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap : 2;
        while (new_cap <= (size_t)n + 1) new_cap <<= 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) return 0;   /* allocation failed */
        cache = tmp;
        cache_cap = new_cap;
    }

    if ((size_t)n >= cache_size) {
        for (size_t i = cache_size; i <= (size_t)n; ++i) {
            seq_t a_im2 = cache[i - 2];
            seq_t a_im1 = cache[i - 1];
            cache[i] = (seq_t)72 * a_im2 - a_im1;
        }
        cache_size = (size_t)n + 1;
    }

    return cache[(size_t)n];
#endif
}
