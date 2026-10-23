/*
 * A028020  Expansion of 1/((1-2x)(1-8x)(1-11x)(1-12x)).
 * https://oeis.org/A028020
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
 *   a(0)=1, a(1)=33, a(2)=711, a(3)=12677
 *   a(n)=33*a(n-1)-378*a(n-2)+1688*a(n-3)-2112*a(n-4)   for n>=4
 */
#include <stdlib.h>

#if defined(__SIZEOF_INT128__)
#define USE_INT128 1
#else
#define USE_INT128 0
#endif

static seq_t *cache = NULL;          /* cached sequence values */
static size_t   cache_size = 0;       /* allocated entries in cache */

static inline seq_t mul64(seq_t a, unsigned long long c)
{
#if USE_INT128
    __uint128_t prod = (__uint128_t)a * c;
    return (seq_t)prod;
#else
    return a * c;
#endif
}

seq_t a(idx_t n)
{
    /* Handle small indices directly */
    if (n < 4) {
        static const seq_t init[4] = { (seq_t)1, (seq_t)33,
                                      (seq_t)711, (seq_t)12677 };
        return init[n];
    }

    /* Initialize cache if needed */
    if (!cache) {
        cache_size = 4;
        cache = (seq_t*)malloc(cache_size * sizeof(seq_t));
        if (!cache) return 0;            /* out of memory: caller may handle */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)33;
        cache[2] = (seq_t)711;
        cache[3] = (seq_t)12677;
    }

    /* Grow cache to accommodate requested index */
    if ((size_t)n >= cache_size) {
        size_t new_size = (size_t)n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) return 0;            /* out of memory */
        cache = tmp;
        for (idx_t k = (idx_t)cache_size; k <= n; ++k) {
            seq_t term1 = mul64(cache[k-1], 33ULL) + mul64(cache[k-3], 1688ULL);
            seq_t term2 = mul64(cache[k-2], 378ULL) + mul64(cache[k-4], 2112ULL);
            cache[k] = term1 - term2;  /* safe: term1 >= term2 for valid n */
        }
        cache_size = new_size;
    }

    return cache[(size_t)n];
}
