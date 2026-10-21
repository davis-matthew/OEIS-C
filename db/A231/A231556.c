/*
 * A231556  G.f. satisfies: A(x) = (1 - x*A(x))^2 * (2*A(x) - 1).
 * https://oeis.org/A231556
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) satisfies the polynomial recurrence
 *            8*n*(n+1)*(55*n-69)*a(n) = 4*n*(1210*n^2-2123*n+771)*a(n-1) -
 *            (1375*n^3-4475*n^2+4146*n-1008)*a(n-2) +
 *            2*(n-3)*(2*n-3)*(55*n-14)*a(n-3). Initial values: a(0)=1,
 *            a(1)=2, a(2)=11
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 21, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#ifdef __cplusplus
#include <type_traits>
#endif

/* cache of computed terms */
static seq_t *cache = NULL;
static size_t cache_size = 0;
static size_t cache_cap   = 0; /* capacity of the cache array */

/* allocate and grow cache array */
static void grow_cache(size_t new_cap)
{
    seq_t *new_cache = NULL;
#if defined(__cplusplus)
    new_cache = new seq_t[new_cap];
#else
    new_cache = (seq_t*)malloc(new_cap * sizeof(seq_t));
#endif
    if (!new_cache) return; /* out of memory */
    for (size_t i = 0; i < cache_size; ++i)
        new_cache[i] = cache[i];
#if defined(__cplusplus)
    delete[] cache;
#else
    free(cache);
#endif
    cache = new_cache;
    cache_cap = new_cap;
}

/* ensure that cache contains terms up to index n (inclusive) */
static void ensure_cache(idx_t n)
{
    size_t need = (size_t)n + 1;
    if (need <= cache_size) return;

    /* grow cache to at least 'need' */
    size_t new_cap = cache_cap ? cache_cap : 4;
    while (new_cap < need) new_cap <<= 1;
    grow_cache(new_cap);

    /* compute missing terms */
    for (size_t i = cache_size; i < need; ++i) {
        if (i == 0) { cache[i] = 1; continue; }
        if (i == 1) { cache[i] = 2; continue; }
        if (i == 2) { cache[i] = 11; continue; }

#if defined(__cplusplus)
        /* C++ version: choose integral or arbitrary‑precision path */
        if constexpr (std::is_integral<seq_t>::value) {
            using int128 = unsigned __int128;
            int128 N = i;
            int128 N2 = N * N;
            int128 leftCoeff = (int128)1210 * N2 - (int128)2123 * N + 771;
            int128 term1 = (int128)4 * N * leftCoeff * (int128)cache[i-1];
            int128 n3 = N * N2;
            int128 coeff2 = (int128)1375 * n3 - (int128)4475 * N2 + (int128)4146 * N - 1008;
            int128 term2 = coeff2 * (int128)cache[i-2];
            int128 coeff3 = (int128)2 * (N-3) * (2*N-3) * ((int128)55 * N - 14);
            int128 term3 = coeff3 * (int128)cache[i-3];
            int128 numerator = term1 - term2 + term3;
            int128 denominator = (int128)8 * N * (N+1) * ((int128)55 * N - 69);
            cache[i] = (seq_t)(numerator / denominator);
        }
        else {
            /* arbitrary‑precision seq_t: use its own operators */
            seq_t N = (seq_t)i;
            seq_t N2 = N * N;
            seq_t leftCoeff = (seq_t)1210 * N2 - (seq_t)2123 * N + 771;
            seq_t term1 = (seq_t)4 * N * leftCoeff * cache[i-1];
            seq_t n3 = N * N2;
            seq_t coeff2 = (seq_t)1375 * n3 - (seq_t)4475 * N2 + (seq_t)4146 * N - 1008;
            seq_t term2 = coeff2 * cache[i-2];
            seq_t coeff3 = (seq_t)2 * (N-3) * (seq_t)(2*N-3) * ((seq_t)55 * N - 14);
            seq_t term3 = coeff3 * cache[i-3];
            seq_t numerator = term1 - term2 + term3;
            seq_t denominator = (seq_t)8 * N * (N+1) * ((seq_t)55 * N - 69);
            cache[i] = numerator / denominator; /* exact division */
        }
#else
        /* C implementation: seq_t is an unsigned integer type.
           Use unsigned __int128 for all intermediate values. */
        unsigned __int128 N   = i;
        unsigned __int128 N2  = N * N;
        unsigned __int128 leftCoeff = (unsigned __int128)1210 * N2
                                - (unsigned __int128)2123 * N + 771;
        unsigned __int128 term1 = (unsigned __int128)4 * N * leftCoeff
                                 * (unsigned __int128)cache[i-1];
        unsigned __int128 n3    = N * N2;
        unsigned __int128 coeff2 = (unsigned __int128)1375 * n3
                                - (unsigned __int128)4475 * N2
                                + (unsigned __int128)4146 * N - 1008;
        unsigned __int128 term2 = coeff2 * (unsigned __int128)cache[i-2];
        unsigned __int128 coeff3 = (unsigned __int128)2 * (N-3) * (2*N-3)
                                  * ((unsigned __int128)55 * N - 14);
        unsigned __int128 term3 = coeff3 * (unsigned __int128)cache[i-3];
        unsigned __int128 numerator   = term1 - term2 + term3;
        unsigned __int128 denominator = (unsigned __int128)8 * N * (N+1)
                                       * ((unsigned __int128)55 * N - 69);
        cache[i] = (seq_t)(numerator / denominator);
#endif
    }
    cache_size = need;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[(size_t)n];
}
