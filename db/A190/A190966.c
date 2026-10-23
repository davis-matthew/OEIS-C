/*
 * A190966  a(n) = 4*a(n-1) - 8*a(n-2), with a(0)=0, a(1)=1.
 * https://oeis.org/A190966
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 4*a(n-1) - 8*a(n-2), with a(0)=0, a(1)=1
 * Range:     i8 n <= 5, i16 n <= 10, i32 n <= 21, i64 n <= 42, i128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#ifdef __cplusplus
#include <type_traits>
static inline seq_t mul4(seq_t x) {
    if constexpr (std::is_integral<seq_t>::value) {
        return (seq_t)((__int128)4 * x);
    } else {
        return 4 * x;
    }
}
static inline seq_t mul8(seq_t x) {
    if constexpr (std::is_integral<seq_t>::value) {
        return (seq_t)((__int128)8 * x);
    } else {
        return 8 * x;
    }
}
#else
#if defined(__SIZEOF_INT128__)
static inline seq_t mul4(seq_t x) { return (seq_t)((__int128)4 * x); }
static inline seq_t mul8(seq_t x) { return (seq_t)((__int128)8 * x); }
#else
static inline seq_t mul4(seq_t x) { return 4 * x; }
static inline seq_t mul8(seq_t x) { return 8 * x; }
#endif
#endif

static seq_t *cache = NULL;
static idx_t cache_cap = 0;
static idx_t max_computed = 0;

static void ensure_cap(idx_t n) {
    if (cache_cap >= (size_t)(n + 1)) return;
    size_t new_cap = cache_cap > 0 ? cache_cap : 16;
    while (new_cap <= (size_t)n) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) exit(EXIT_FAILURE);
    cache = tmp;
    cache_cap = new_cap;
}

seq_t a(idx_t n) {
    /* Ensure storage for indices up to n */
    ensure_cap(n);

    /* Initialize base terms if needed */
    if (max_computed == 0) {
        cache[0] = (seq_t)0;
        if (cache_cap > 1) {
            cache[1] = (seq_t)1;
        } else {
            /* Extend to hold index 1 */
            seq_t *tmp = (seq_t *)realloc(cache, 2 * sizeof(seq_t));
            if (!tmp) exit(EXIT_FAILURE);
            cache = tmp;
            cache_cap = 2;
            cache[1] = (seq_t)1;
        }
        max_computed = 1;
    }

    /* Extend computation up to n if necessary */
    if (n > max_computed) {
        idx_t i;
        for (i = max_computed + 1; i <= n; ++i) {
            cache[i] = mul4(cache[i - 1]) - mul8(cache[i - 2]);
        }
        max_computed = n;
    }

    return cache[n];
}
