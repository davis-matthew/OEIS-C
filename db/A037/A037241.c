/*
 * A037241  Expansion of
 *          (1+3*x^2+7*x^3+15*x^4+13*x^5+15*x^6+8*x^7+4*x^8)/((1-x)*(1-x^2)^3*(1-x^3)^2).
 * https://oeis.org/A037241
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = coefficient of x^n in
 *            (1+3*x^2+7*x^3+15*x^4+13*x^5+15*x^6+8*x^7+4*x^8) / divided by
 *            (1-x)*(1-x^2)^3*(1-x^3)^2
 * Range:     u8 n <= 6, u16 n <= 22, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Validated: partial, pass, 38 terms through n = 37 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* Helper allocation for seq_t (arbitrary‑precision or builtin integer) */
static seq_t* seq_alloc(size_t cap) {
#ifdef __cplusplus
    return new seq_t[cap];
#else
    return (seq_t*)malloc(cap * sizeof(seq_t));
#endif
}

static seq_t* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap) {
#ifdef __cplusplus
    seq_t* new_ptr = new seq_t[new_cap];
    for (size_t i = 0; i < old_cap; ++i)
        new_ptr[i] = old[i];
    delete[] old;
    return new_ptr;
#else
    return (seq_t*)realloc(old, new_cap * sizeof(seq_t));
#endif
}

static const int d = 13;
static const int q[14] = {1, -1, -3, 1, 5, 3, -6, -6, 3, 5, 1, -3, -1, 1};
static const int P[9] = {1, 0, 3, 7, 15, 13, 15, 8, 4};

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t cache_len = 0;

    if ((size_t)n >= cache_cap) {
        size_t new_cap = ((size_t)n + 1) * 2;
        if (new_cap < 16) new_cap = 16;
        if (cache == NULL)
            cache = seq_alloc(new_cap);
        else
            cache = seq_realloc(cache, cache_cap, new_cap);
        cache_cap = new_cap;
    }

    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t val = (i <= 8) ? (seq_t)P[i] : (seq_t)0;
        for (int k = 1; k <= d; ++k) {
            if (i >= (idx_t)k) {
                seq_t prod = cache[i - (idx_t)k] * (seq_t)abs(q[k]);
                if (q[k] > 0)  /* subtract positive coefficient */
                    val -= prod;
                else           /* add when coefficient is negative */
                    val += prod;
            }
        }
        cache[i] = val;
    }
    cache_len = n + 1;
    return cache[n];
}
