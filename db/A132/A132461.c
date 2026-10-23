/*
 * A132461  Row squared sums of triangle of Lucas polynomials (A034807) for
 *          n>0: Sum_{k=0..floor(n/2)} A034807(n,k)^2, with a(0)=1.
 * https://oeis.org/A132461
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=1, a(2)=5, a(3)=10, a(4)=21 For n>=5: (n-3)*n*a(n)
 *            = (2*n^2 - 7*n + 4)*a(n-1) + (n-4)*n*a(n-2) + (2*n^2 - 9*n +
 *            8)*a(n-3) - (n-4)*(n-1)*a(n-4)
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 24, u64 n <= 48, u128 n <= 88,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#ifndef __cplusplus
#include <stdlib.h>
#include <stddef.h>

static const seq_t init_vals[5] = { (seq_t)1, (seq_t)1, (seq_t)5, (seq_t)10, (seq_t)21 };

static inline seq_t compute_term(idx_t i, seq_t *c)
{
    unsigned __int128 num = 0;
    unsigned __int128 nseq = (unsigned __int128)i;

    unsigned __int128 coeff1 = (unsigned __int128)2 * nseq * nseq
                              - (unsigned __int128)7 * nseq
                              + (unsigned __int128)4;

    unsigned __int128 coeff2 = (unsigned __int128)(i - 4) * nseq;
    unsigned __int128 coeff3 = (unsigned __int128)2 * nseq * nseq
                              - (unsigned __int128)9 * nseq
                              + (unsigned __int128)8;
    unsigned __int128 coeff4 = (unsigned __int128)(i - 4) * (unsigned __int128)(i - 1);

    num += coeff1 * (unsigned __int128)c[i - 1];
    num += coeff2 * (unsigned __int128)c[i - 2];
    num += coeff3 * (unsigned __int128)c[i - 3];
    num -= coeff4 * (unsigned __int128)c[i - 4];

    unsigned __int128 denom = (unsigned __int128)(i - 3) * nseq;
    return (seq_t)(num / denom);
}

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_used = 0;
    static size_t cache_cap = 0;

    if (n < 5)
        return init_vals[n];

    if (cache == NULL) {
        cache_cap = (size_t)((n > 4) ? n + 1 : 5);
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        for (idx_t i = 0; i < 5; ++i)
            cache[i] = init_vals[i];
        cache_used = 5;
    } else if (cache_cap <= n) {
        size_t new_cap = (size_t)(n + 1);
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) exit(1);
        cache = tmp;
        cache_cap = new_cap;
    }

    for (idx_t i = cache_used; i <= n; ++i)
        cache[i] = compute_term(i, cache);

    cache_used = n + 1;
    return cache[n];
}
#else /* __cplusplus */
#include <cstdlib>
#include <type_traits>

static const seq_t init_vals[5] = { (seq_t)1, (seq_t)1, (seq_t)5, (seq_t)10, (seq_t)21 };

template <typename T>
typename std::enable_if<std::is_integral<T>::value, T>::type
compute_term(idx_t i, T *c)
{
    using big = unsigned __int128;
    big num = 0;
    big nseq = (big)i;

    big coeff1 = (big)2 * nseq * nseq
                 - (big)7 * nseq
                 + (big)4;
    big coeff2 = (big)(i - 4) * nseq;
    big coeff3 = (big)2 * nseq * nseq
                 - (big)9 * nseq
                 + (big)8;
    big coeff4 = (big)(i - 4) * (big)(i - 1);

    num += coeff1 * (big)c[i - 1];
    num += coeff2 * (big)c[i - 2];
    num += coeff3 * (big)c[i - 3];
    num -= coeff4 * (big)c[i - 4];

    big denom = (big)(i - 3) * nseq;
    return (T)(num / denom);
}

template <typename T>
typename std::enable_if<!std::is_integral<T>::value, T>::type
compute_term(idx_t i, T *c)
{
    T num = (T)0;
    T nseq = (T)i;

    T coeff1 = (T)2 * nseq * nseq - (T)7 * nseq + (T)4;
    T coeff2 = (T)(i - 4) * nseq;
    T coeff3 = (T)2 * nseq * nseq - (T)9 * nseq + (T)8;
    T coeff4 = (T)(i - 4) * (T)(i - 1);

    num = coeff1 * c[i - 1];
    num += coeff2 * c[i - 2];
    num += coeff3 * c[i - 3];
    num -= coeff4 * c[i - 4];

    T denom = (T)(i - 3) * nseq;
    return num / denom;
}

seq_t a(idx_t n)
{
    static seq_t *cache = nullptr;
    static idx_t cache_used = 0;
    static size_t cache_cap = 0;

    if (n < 5)
        return init_vals[n];

    if (cache == nullptr) {
        cache_cap = (size_t)((n > 4) ? n + 1 : 5);
        cache = (seq_t*)std::malloc(cache_cap * sizeof(seq_t));
        for (idx_t i = 0; i < 5; ++i)
            cache[i] = init_vals[i];
        cache_used = 5;
    } else if (cache_cap <= n) {
        size_t new_cap = (size_t)(n + 1);
        seq_t *tmp = (seq_t*)std::realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == nullptr) std::exit(1);
        cache = tmp;
        cache_cap = new_cap;
    }

    for (idx_t i = cache_used; i <= n; ++i)
        cache[i] = compute_term(i, cache);

    cache_used = n + 1;
    return cache[n];
}
#endif /* __cplusplus */
