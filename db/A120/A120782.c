/*
 * A120782  Numerators of partial sums of Catalan numbers scaled by powers of
 *          1/12.
 * https://oeis.org/A120782
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = numerator of r(n), r(n) = Sum_{k=0..n} Catalan(k)/12^k
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Validated: partial, pass, 18 terms through n = 17 (2026-11-03)
 * Helpers:   catalan, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Requires the helper function catalan(idx_t k) from utils. */
#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/catalan.h"
#include "../utils/mem.h"

#ifdef __cplusplus
#define OEISC_VPP(p) (p)
#define OEISC_VP(p) (p)
#else
#define OEISC_VPP(p) ((void **)(p))
#define OEISC_VP(p) ((void *)(p))
#endif

/* multiply a seq_t by a small uint32_t constant without overflow
 * for 64‑bit seq_t we use unsigned __int128 as intermediate,
 * for larger seq_t we rely on the built‑in arithmetic. */
static inline seq_t mul_small(seq_t x, unsigned int m)
{
    if (sizeof(seq_t) <= 8) {
        unsigned __int128 res = (unsigned __int128)x * m;
        return (seq_t)res;
    } else {
        return x * m;
    }
}

/* Grow a dynamic array when needed.  The pointer is passed as OEISC_VPP(.) */
#ifdef __cplusplus
template <class T>
static void ensure_capacity(size_t needed, size_t *cap, T **ptr, size_t elem_size)
{
    if (needed <= *cap) return;
    size_t new_cap = (*cap == 0) ? 4 : (*cap << 1);
    while (new_cap < needed) new_cap <<= 1;
    T *tmp = mem_resize(T, *ptr, (new_cap * elem_size) / sizeof(T));
    if (!tmp) abort();               /* on OOM we abort as required */
    *ptr = tmp;
    *cap = new_cap;
}
#else
static void ensure_capacity(size_t needed, size_t *cap, void **ptr, size_t elem_size)
{
    if (needed <= *cap) return;
    size_t new_cap = (*cap == 0) ? 4 : (*cap << 1);
    while (new_cap < needed) new_cap <<= 1;
    void *tmp = realloc(*ptr, new_cap * elem_size);
    if (!tmp) abort();               /* on OOM we abort as required */
    *ptr = tmp;
    *cap = new_cap;
}
#endif

/* Multiplication by a small integer (2 or 3). */

/* Utility to ensure capacity of a dynamic array. */

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* Static cache holding numerators of all computed terms. */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t cache_len = 0;

    /* Current state of the accumulated fraction r(k) for most recently
       computed k.  num  = numerator,  a_exp = exponent of 2 in denom,
       b_exp = exponent of 3 in denom.  The denominator after reduction
       is 2^a_exp * 3^b_exp.  This is the value of r(cache_len). */
    static seq_t cur_num = 0;
    static idx_t cur_a = 0, cur_b = 0;

    /* ---------- Initialisation ---------- */
    if (cache_len == 0) {
        /* Allocate cache for some initial capacity. */
        ensure_capacity(1, &cache_cap, OEISC_VPP(&cache), sizeof(seq_t));
        /* r(0) = 1/1, so numerator is 1. */
        cache[0] = (seq_t)1;
        cache_len = 1;
        cur_num = (seq_t)1;
        cur_a   = 0;   /* 2^0 */
        cur_b   = 0;   /* 3^0 */
    }

    /* ---------- Return cached value if already computed ---------- */
    if (n < cache_len)
        return cache[(size_t)n];

    /* ---------- Compute terms up to n ---------- */
    for (idx_t k = cache_len; k <= n; ++k) {
        /* For k=cache_len (i.e., the next term after the last computed),
           the current state corresponds to k-1.  Compute the necessary
           exponents of 2 and 3 that must multiply cur_num to bring the
           denominator to 12^k.  12^k = 2^(2k) * 3^k. */
        idx_t factor_exp_2 = 2 * k - cur_a;   /* 2^ (2k - a_prev) */
        idx_t factor_exp_3 = k - cur_b;       /* 3^ (k - b_prev) */

        /* Start with the previously accumulated numerator. */
        seq_t newnum = cur_num;

        /* Multiply by 2 power. */
        for (idx_t i = 0; i < factor_exp_2; ++i)
            newnum = mul_small(newnum, 2);

        /* Multiply by 3 power. */
        for (idx_t i = 0; i < factor_exp_3; ++i)
            newnum = mul_small(newnum, 3);

        /* Add the new term: Catalan(k)/12^k. */
        newnum += catalan(k);

        /* After addition, the denominator is 12^k = 2^(2k) * 3^k. */
        idx_t new_a = 2 * k;
        idx_t new_b = k;

        /* Reduce the fraction by cancelling common factors of 2 and 3. */
        while ((newnum % 2) == 0 && new_a > 0) {
            newnum /= 2;
            --new_a;
        }
        while ((newnum % 3) == 0 && new_b > 0) {
            newnum /= 3;
            --new_b;
        }

        /* Store the computed numerator. */
        ensure_capacity((size_t)(k + 1), &cache_cap, OEISC_VPP(&cache), sizeof(seq_t));
        cache[(size_t)k] = newnum;

        /* Update state for next iteration. */
        cur_num = newnum;
        cur_a   = new_a;
        cur_b   = new_b;
    }

    cache_len = n + 1;
    return cache[(size_t)n];
}
