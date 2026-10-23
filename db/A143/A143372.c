/*
 * A143372  Expansion of 1/(1 +x -2*x^2 -x^3 -x^4 -3*x^5 +2*x^6 +2*x^7 +3*x^8
 *          +2*x^9 -3*x^10 -7*x^11 -3*x^12 -5*x^13).
 * https://oeis.org/A143372
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 9, i16 n <= 24, i32 n <= 48, i64 n <= 98, i128 n <= 196,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A143372 – Expansion of 1/(1 +x -2*x^2 -x^3 -x^4 -3*x^5 +2*x^6
    +2*x^7 +3*x^8 +2*x^9 -3*x^10 -7*x^11 -3*x^12 -5*x^13)

    Recurrence (assumes a(0)=1):
    a(n) = -a(n-1) + 2*a(n-2) + a(n-3) + a(n-4) + 3*a(n-5)
           - 2*a(n-6) - 2*a(n-7) - 3*a(n-8) - 2*a(n-9)
           + 3*a(n+10) + 7*a(n+11) + 3*a(n+12) + 5*a(n+13)
    (for n ≥ 1; terms with negative indices are treated as 0).  */

#include <stdlib.h>

#ifdef __cplusplus
#include <vector>
#endif

/* Coefficients of the denominator polynomial (coeff[0] is omitted) */
static const int d[14] = {
    0,   /* unused  */
    1,  -2,  -1,  -1,  -3,   2,   2,   3,   2,  -3,  -7,  -3,  -5
};

#ifdef __cplusplus
static std::vector<seq_t> cache;
#else
static seq_t *cache = NULL;
static size_t cache_size = 0;
#endif

seq_t a(idx_t n) {
    size_t target = (size_t)n;            /* index we want to compute */

#ifdef __cplusplus
    /* C++ path: use std::vector for dynamic storage */
    if (cache.empty()) {                 /* first call – initialise a(0) */
        cache.resize(1);
        cache[0] = (seq_t)1;
    }
    if (cache.size() <= target) {         /* need to grow and compute new terms */
        cache.resize(target + 1);
        for (size_t i = 1; i <= target; ++i) {
            seq_t sum = 0;
            for (int k = 1; k <= 13; ++k) {
                if (i >= (size_t)k)
                    sum += (seq_t)d[k] * cache[i - k];
            }
            cache[i] = -sum;
        }
    }
    return cache[target];

#else
    /* C path: use malloc / realloc and an explicit size counter */
    if (cache == NULL) {                 /* first call – allocate a(0) */
        cache = (seq_t*)malloc(sizeof(seq_t));
        if (!cache) return 0;            /* unlikely, but defensive */
        cache[0] = (seq_t)1;             /* a(0) = 1 */
        cache_size = 1;
    }
    if (cache_size <= target) {          /* grow array and compute */
        seq_t *new_cache = (seq_t*)realloc(cache, (target + 1) * sizeof(seq_t));
        if (!new_cache) return 0;        /* allocation failed – fallback */
        cache = new_cache;
        for (size_t i = cache_size; i <= target; ++i) {
            seq_t sum = 0;
            for (int k = 1; k <= 13; ++k) {
                if (i >= (size_t)k)
                    sum += (seq_t)d[k] * cache[i - k];
            }
            cache[i] = -sum;
        }
        cache_size = target + 1;
    }
    return cache[target];
#endif
}
