/*
 * A009257  Expansion of e.g.f. exp(tanh(log(1+x))).
 * https://oeis.org/A009257
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = (3-2*n)*a(n-1) + (n-1)*(5-2*n)*a(n-2) +
 *            (1-n)*(n-2)*(n-3)*a(n-3) + (1-n)*(n-2)*(n-3)*(n-4)*a(n-4)/4
 *            (n>3) Initial values: a(0)=1, a(1)=1, a(2)=0, a(3)=-2
 * Range:     i8 n <= 6, i16 n <= 9, i32 n <= 14, i64 n <= 22, i128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Necessary includes for both C11 and C++17 compilations */
#include <stdlib.h>
#include <stdint.h>

#ifdef __cplusplus
#include <type_traits>
/* Multiplication of a seq_t value by an __int128 coefficient.
   For integral seq_t types, use 128‑bit arithmetic;
   otherwise rely on the seq_t multiplication operator. */
static inline seq_t mul128(const seq_t& x, __int128 coeff) {
    if constexpr (std::is_integral_v<seq_t>) {
        return (seq_t)(((__int128)x) * coeff);
    } else {
        /* assume the seq_t type can be constructed from a signed long long */
        return x * (seq_t)coeff;
    }
}
#else
/* In plain C, seq_t is a builtin integer type. */
#define mul128(x,coeff) ((seq_t)(((__int128)(x)) * coeff))
#endif

seq_t a(idx_t n) {
    /* Static memoisation cache */
    static seq_t *cache = NULL;
    static size_t  cache_size = 0;   /* number of terms already filled */
    static size_t  cache_cap  = 0;   /* allocated capacity */

    /* Lazy initialisation with the first four terms */
    if (!cache) {
        cache_cap  = 8;                     /* at least large enough for 4 terms */
        cache      = (seq_t *)calloc(cache_cap, sizeof(seq_t));
        if (!cache) return 0;               /* allocation failed */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)0;
        cache[3] = (seq_t)(-2);
        cache_size = 4;
    }

    if (n < cache_size)
        return cache[n];

    /* Grow the cache capacity if necessary */
    if (n + 1 > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap : 8;
        while (new_cap <= (size_t)(n + 1))
            new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0; /* allocation failed */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute new terms up to n */
    for (idx_t i = cache_size; i <= n; ++i) {
        if (i <= 3) {
            /* The initial values are already stored */
            continue;
        }

        seq_t a1 = cache[i-1];
        seq_t a2 = cache[i-2];
        seq_t a3 = cache[i-3];
        seq_t a4 = cache[i-4];

        /* Use __int128 for the coefficients to avoid overflow */
        __int128 ni   = ( __int128 )i;
        __int128 c1 = ( (__int128)3 - (__int128)2 * ni );
        __int128 c2 = ( (__int128)(ni-1) * ( (__int128)5 - (__int128)2 * ni ) );
        __int128 c3 = ( (__int128)(1-ni) * ( (__int128)(ni-2) ) * ( (__int128)(ni-3) ) );
        __int128 c4 = ( (__int128)(1-ni) * ( (__int128)(ni-2) ) *
                        ( (__int128)(ni-3) ) * ( (__int128)(ni-4) ) ) / (__int128)4;

        seq_t t1 = mul128(a1, c1);
        seq_t t2 = mul128(a2, c2);
        seq_t t3 = mul128(a3, c3);
        seq_t t4 = mul128(a4, c4);

        cache[i] = t1 + t2 + t3 + t4;
    }

    cache_size = (size_t)(n + 1);
    return cache[n];
}
