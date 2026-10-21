/*
 * A228800  Number of 5Xn binary arrays with top left element equal to 1 and
 *          no two ones adjacent horizontally or nw-se.
 * https://oeis.org/A228800
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A228800  */
/*  Number of 5×n binary arrays with top left element equal to 1 and  
 *  no two ones adjacent horizontally or nw‑se.
 *
 *  Recurrence (empirical):
 *  a(n) = a(n-1) +49*a(n-2) +107*a(n-3) -154*a(n-4) -404*a(n-5) +250*a(n-6)
 *         +472*a(n-7) -278*a(n-8) -168*a(n-9) +131*a(n-10) -9*a(n-11)
 *         -7*a(n-12) +a(n-13)
 *
 *  Initial values:
 *    a(1)=16, a(2)=34, a(3)=672, a(4)=3588, a(5)=37944, a(6)=272952,
 *    a(7)=2404290, a(8)=19027372, a(9)=158932704, a(10)=1292370748,
 *    a(11)=10646457736, a(12)=87161916810, a(13)=715678163448
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* ---------- Initial known terms ---------- */
    static const seq_t init[13] = {
        /*  n  terms  */
        16ULL, 34ULL, 672ULL, 3588ULL, 37944ULL, 272952ULL,
        2404290ULL, 19027372ULL, 158932704ULL, 1292370748ULL,
        10646457736ULL, 87161916810ULL, 715678163448ULL
    };

    /* ---------- Static cache holding computed terms ---------- */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;      /* number of terms stored */
    static size_t cache_cap = 0;      /* allocated capacity */

    /* Grow the cache to the required capacity.  Works for both C
     * and C++ (using new[] for the latter).  All terms are value
     * types, so this is safe. */
    const size_t required_cap = (size_t)n;
    if (required_cap > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        if (new_cap < required_cap) new_cap = required_cap;
#ifdef __cplusplus
        seq_t *new_cache = new seq_t[new_cap];
        for (size_t i = 0; i < cache_len; ++i) new_cache[i] = cache[i];
        delete[] cache;
#else
        seq_t *new_cache = (seq_t *)realloc(cache,
                                            new_cap * sizeof(seq_t));
        if (!new_cache) abort(); /* allocation failed */
        cache = new_cache;
#endif
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* Initialise cache with the 13 known terms on first call */
    if (cache_len == 0) {
        memcpy(cache, init, sizeof(init));
        cache_len = 13;
    }

    /* Return the term if already computed */
    if (n <= cache_len) return cache[(size_t)(n - 1)];

    /* Coefficients of the recurrence */
    static const int coeffs[13] = {
        1, 49, 107, -154, -404, 250, 472, -278, -168,
        131, -9, -7, 1
    };

    /* Compute the terms from cache_len+1 up to n */
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        seq_t next = 0;
        for (int k = 0; k < 13; ++k) {
            idx_t idx = i - k - 2;                  /* i‑1 gives k=0 → i‑2 */
            seq_t term = cache[(size_t)idx];
            int coeff = coeffs[k];
            seq_t ac = (seq_t)(coeff >= 0 ? coeff : -coeff);
            if (coeff >= 0)
                next += ac * term;
            else
                next -= ac * term;
        }
        cache[(size_t)(i - 1)] = next;
    }
    cache_len = n;
    return cache[(size_t)(n - 1)];
}
