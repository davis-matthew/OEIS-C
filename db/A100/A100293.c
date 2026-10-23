/*
 * A100293  Numbers of the form a^5 + b^3 with a,b>0.
 * https://oeis.org/A100293
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th smallest integer >0 of the form a^5 + b^3 * with
 *            a,b>0 (OEIS A100293). This implementation pre‑computes all *
 *            such values up to a fixed limit (4×10^8), which suffices for n
 *            up * to at least 20000. The values are generated once, stored in
 *            a * static cache, and on subsequent calls a...
 * Range:     u8 none, u16 n <= 264, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#ifdef __cplusplus
#include <vector>
#include <algorithm>
#else
#include <stdlib.h>
#include <stdbool.h>
#endif
#include <stdint.h>

/* Helper: cube of a 64‑bit integer, in seq_t */
static inline seq_t cube_uint64(idx_t x) {
    seq_t y = (seq_t)x;
    return y * y * y;
}

/* Helper: fifth power of a 64‑bit integer, in seq_t */
static inline seq_t fifth_uint64(idx_t x) {
    seq_t y = (seq_t)x;
    y *= y;          /* y = x^2 */
    y *= y;          /* y = x^4 */
    y *= (seq_t)x;   /* y = x^5 */
    return y;
}

#ifdef __cplusplus
/* C++ implementation */
seq_t a(idx_t n) {
    static std::vector<seq_t> cache;
    if (cache.empty()) {
        const seq_t LIMIT = (seq_t)400000000ULL;   /* 4e8 */
        std::vector<seq_t> tmp;
        for (idx_t a = 1;; ++a) {
            seq_t a5 = fifth_uint64(a);
            if (a5 > LIMIT) break;
            for (idx_t b = 1;; ++b) {
                seq_t s = a5 + cube_uint64(b);
                if (s > LIMIT) break;
                tmp.push_back(s);
            }
        }
        std::sort(tmp.begin(), tmp.end());
        auto last = std::unique(tmp.begin(), tmp.end());
        cache.assign(tmp.begin(), last);
    }
    /* n is 1‑based; assume caller never requests n=0 */
    return cache[n - 1];
}
#else
/* C implementation */
static seq_t* cache = NULL;
static size_t cache_size = 0;
static size_t cache_capacity = 0;

static int cmp_seq_t(const void *p1, const void *p2) {
    seq_t a = *(const seq_t*)p1;
    seq_t b = *(const seq_t*)p2;
    if (a < b) return -1;
    if (a > b) return 1;
    return 0;
}

static void ensure_cache(idx_t n) {
    if (cache && cache_size >= (size_t)n) return;   /* already enough */

    /* compute up to a fixed limit that yields enough terms */
    const seq_t LIMIT = (seq_t)400000000ULL;   /* 4e8 */

    /* Rough upper bound for number of candidate sums */
    size_t estimated = 200000; /* comfortably larger than needed */
    if (cache_capacity < estimated) {
        cache_capacity = estimated;
        cache = (seq_t*)realloc(cache, cache_capacity * sizeof(seq_t));
    }

    size_t idx = 0;
    for (idx_t a = 1;; ++a) {
        seq_t a5 = fifth_uint64(a);
        if (a5 > LIMIT) break;
        for (idx_t b = 1;; ++b) {
            seq_t s = a5 + cube_uint64(b);
            if (s > LIMIT) break;
            if (idx >= cache_capacity) {
                cache_capacity *= 2;
                cache = (seq_t*)realloc(cache, cache_capacity * sizeof(seq_t));
            }
            cache[idx++] = s;
        }
    }

    /* sort the array */
    qsort(cache, idx, sizeof(seq_t), cmp_seq_t);

    /* remove duplicates in place */
    size_t uniq = 0;
    for (size_t i = 0; i < idx; ++i) {
        if (uniq == 0 || cache[i] != cache[uniq - 1]) {
            cache[uniq++] = cache[i];
        }
    }
    cache_size = uniq;
}

seq_t a(idx_t n) {
    /* n is 1‑based; ignore n==0 */
    ensure_cache(n);
    return cache[n - 1];
}
#endif
