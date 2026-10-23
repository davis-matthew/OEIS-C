/*
 * A049397  Expansion of (1-25*x)^(-9/5).
 * https://oeis.org/A049397
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5^n/n! * ∏_{k=0}^{n-1} (5k+9) Recurrence: a(0)=1,
 *            a(n)=a(n-1)*5*(5*(n-1)+9)/n
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

#if defined(__cplusplus)
#include <type_traits>
#endif

/* Helper: multiply by num and divide by den using a 128‑bit intermediate
   for integral types, otherwise use normal arithmetic (works for arbitrary
   precision types). */
static inline seq_t mult_div(seq_t a, seq_t num, idx_t den)
{
#if defined(__cplusplus)
    if constexpr (std::is_arithmetic<seq_t>::value) {
        __uint128_t temp = (__uint128_t)a * (__uint128_t)num;
        return (seq_t)(temp / den);
    } else {
        seq_t tmp = a * num;
        return tmp / den;
    }
#else
    /* C16/C++: assume seq_t is an integral type */
    __uint128_t temp = (__uint128_t)a * (__uint128_t)num;
    return (seq_t)(temp / den);
#endif
}

/* Memoized sequence */
static seq_t *cache       = NULL;
static idx_t  cache_size  = 0;
static idx_t  cache_cap   = 0;

seq_t a(idx_t n)
{
    /* Allocate or grow cache if necessary */
    if (n >= cache_cap) {
        idx_t new_cap = n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, (size_t)new_cap * sizeof(seq_t));
        if (new_cache == NULL) abort();      /* allocation failed */
        cache      = new_cache;
        cache_cap  = new_cap;
    }

    /* Compute new terms lazily */
    for (idx_t i = cache_size; i <= n; ++i) {
        if (i == 0) {
            cache[0] = 1;
        } else {
            /* num = 5 * (5*(i-1) + 9) */
            seq_t num = (seq_t)5 * (seq_t)(5 * (i - 1) + 9);
            cache[i]  = mult_div(cache[i - 1], num, i);
        }
    }
    cache_size = n + 1;
    return cache[n];
}
