/*
 * A104237  Expansion of (1 - x + x^2)*(1 + x + x^2 - x^3 + 2*x^4)/((1 - x)*(1
 *          + x)^2*(1 + x^2)*(1 + x - x^2 + x^3)).
 * https://oeis.org/A104237
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = -2*a(n-1) + 2*a(n-5) + a(n-8) for n > 7 Initial values:
 *            a(0)=1, a(1)=-2, a(2)=5, a(3)=-11, a(4)=26, a(5)=-53, a(6)=104,
 *            a(7)=-198
 * Range:     i8 n <= 6, i16 n <= 15, i32 n <= 33, i64 n <= 69, i128 n <= 142,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t base[8] = {1, -2, 5, -11, 26, -53, 104, -198};

#ifdef __cplusplus
#include <vector>
static std::vector<seq_t> cache;
seq_t a(idx_t n) {
    size_t idx = (size_t)n;
    if (cache.empty()) {
        cache.resize(8);
        for (size_t i = 0; i < 8; ++i) cache[i] = base[i];
    }
    size_t cur = cache.size();
    if (idx + 1 > cur) {
        cache.resize(idx + 1);
        for (size_t i = cur; i <= idx; ++i) {
            cache[i] = -2 * cache[i - 1] + 2 * cache[i - 5] + cache[i - 8];
        }
    }
    return cache[idx];
}
#else
static seq_t *cache = NULL;
static size_t capacity = 0;
static size_t computed = 0; /* number of computed entries */

seq_t a(idx_t n) {
    size_t idx = (size_t)n;
    size_t need = idx + 1;
    if (need > capacity) {
        size_t new_cap = capacity ? capacity * 2 : 8;
        if (new_cap < need) new_cap = need;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) {
            /* Allocation failure – return 0 as a fallback */
            return 0;
        }
        cache = new_cache;
        capacity = new_cap;
    }
    /* Fill basic values if not already done */
    if (computed == 0) {
        size_t end = need < 8 ? need : 8;
        for (size_t i = 0; i < end; ++i) cache[i] = base[i];
        computed = end;
    } else if (computed < 8) {
        size_t end = need < 8 ? need : 8;
        for (size_t i = computed; i < end; ++i) cache[i] = base[i];
        computed = end;
    }
    /* Compute additional terms if required */
    if (idx + 1 > computed) {
        for (size_t i = computed; i <= idx; ++i) {
            cache[i] = -2 * cache[i - 1] + 2 * cache[i - 5] + cache[i - 8];
        }
        computed = idx + 1;
    }
    return cache[idx];
}
#endif
