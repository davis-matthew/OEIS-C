/*
 * A049609  a(n)=T(n,n+3), array T as in A049600.
 * https://oeis.org/A049609
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   n*(n+3)*(n-1)*a(n) -(2*n+1)*(3*n^2+3*n-4)*a(n-1)
 *            +(n-2)*(n+2)*(n+1)*a(n-2)=0; a(0)=0, a(1)=1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#if defined(__cplusplus)
#include <type_traits>
#endif

/* Multiplication helper that prevents overflow on integral types */
static inline seq_t mul_seq(seq_t x, seq_t y)
{
#if defined(__cplusplus)
    if (std::is_integral<seq_t>::value) {
        return (seq_t)(((__uint128_t)x) * ((__uint128_t)y));
    } else {
        return x * y;
    }
#else
    return (seq_t)(((__uint128_t)x) * ((__uint128_t)y));
#endif
}

/* Addition helper that prevents overflow on integral types */
static inline seq_t add_seq(seq_t x, seq_t y)
{
#if defined(__cplusplus)
    if (std::is_integral<seq_t>::value) {
        return (seq_t)(((__uint128_t)x) + ((__uint128_t)y));
    } else {
        return x + y;
    }
#else
    return (seq_t)(((__uint128_t)x) + ((__uint128_t)y));
#endif
}

/* Subtraction helper that prevents underflow on integral types */
static inline seq_t sub_seq(seq_t x, seq_t y)
{
#if defined(__cplusplus)
    if (std::is_integral<seq_t>::value) {
        return (seq_t)(((__uint128_t)x) - ((__uint128_t)y));
    } else {
        return x - y;
    }
#else
    return (seq_t)(((__uint128_t)x) - ((__uint128_t)y));
#endif
}

/* Division helper that works for integral types and arbitrary-precision types */
static inline seq_t div_seq(seq_t num, seq_t den)
{
#if defined(__cplusplus)
    if (std::is_integral<seq_t>::value) {
        return (seq_t)(((__uint128_t)num) / ((__uint128_t)den));
    } else {
        return num / den;
    }
#else
    return (seq_t)(((__uint128_t)num) / ((__uint128_t)den));
#endif
}

/* Compute a(n) with memoisation; works for any idx_t ≥ 0 */
seq_t a(idx_t n)
{
    /* Static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t  cache_max = 0;
    static size_t cache_cap = 0;

    /* Lazy initialisation */
    if (!cache) {
        cache_cap = 3;                        /* space for indices 0..2 */
        cache     = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        cache[0]  = (seq_t)0;
        cache[1]  = (seq_t)1;
        cache[2]  = (seq_t)7;                  /* a(2) from the recurrence */
        cache_max = 2;
    }

    /* Adjust capacity if necessary */
    if (n >= cache_cap) {
        size_t new_cap = cache_cap;
        while (new_cap <= (size_t)n) new_cap <<= 1;   /* grow exponentially */

        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) { /* in case of allocation failure, return 0 (unlikely) */
            return (seq_t)0;
        }
        cache     = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing terms up to n */
    if (n > cache_max) {
        for (idx_t i = cache_max + 1; i <= n; ++i) {
            /* Compute coefficients using idx_t for safety */
            idx_t i1 = i;                      /* i */

            /* term1 = (2*i+1) * (3*i*i + 3*i - 4) * a[i-1] */
            seq_t c1 = (seq_t)(2 * i1 + 1);
            seq_t c2 = (seq_t)(3 * i1 * i1 + 3 * i1 - 4);
            seq_t term1 = mul_seq(mul_seq(c1, c2), cache[i1 - 1]);

            /* term2 = (i-2) * (i+2) * (i+1) * a[i-2] */
            seq_t c3 = (seq_t)(i1 - 2);
            seq_t c4 = (seq_t)(i1 + 2);
            seq_t c5 = (seq_t)(i1 + 1);
            seq_t term2 = mul_seq(mul_seq(mul_seq(c3, c4), c5), cache[i1 - 2]);

            /* Numerator and denominator of the recurrence */
            seq_t numer = sub_seq(term1, term2);
            seq_t denom = mul_seq(mul_seq((seq_t)i1, (seq_t)(i1 + 3)), (seq_t)(i1 - 1));

            cache[i] = div_seq(numer, denom);
        }
        cache_max = n;
    }

    return cache[n];
}
