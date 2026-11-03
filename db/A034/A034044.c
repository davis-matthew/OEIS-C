/*
 * A034044  Numbers that are primitively but not imprimitively represented by
 *          x^2+y^2+z^2.
 * https://oeis.org/A034044
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n‑th positive integer that is a sum of three squares,
 *            primitively represented by x² + y² + z², and not divisible by
 *            any square > 1 whose quotient is also a sum of three squares
 * Range:     u8 n <= 63, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Validated: partial, pass, 63 terms through n = 63 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Necessary standard headers */
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/mem.h"

#ifdef __cplusplus
#define OEISC_VPP(p) (p)
#define OEISC_VP(p) (p)
#else
#define OEISC_VPP(p) ((void **)(p))
#define OEISC_VP(p) ((void *)(p))
#endif

/* Utility for dynamic capacity management, provided by the build system */

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

/* Test whether an integer can be expressed as x²+y²+z².
   Legendre's theorem: n is representable iff n ≠ 4ᵃ(8b+7). */
static inline bool is_sum_three_squares(seq_t n)
{
    while (n % 4 == 0) {
        n /= 4;
    }
    return (n % 8) != 7;
}

/* Main sequence function.  The result is returned in type seq_t,
   and the index is given in type idx_t. */
seq_t a(idx_t n)
{
    /* Cache for previously computed terms */
    static seq_t *cache      = NULL;
    static size_t cache_len  = 0;
    static size_t cache_cap  = 0;
    static idx_t  next = 1;          /* Next integer to test (1‑based) */

    /* Undefined for n = 0: return 0 as a safe placeholder. */
    if (n == 0) {
        return 0;
    }

    /* Extend the cache until we have at least n terms. */
    while (cache_len < n) {
        seq_t m = (seq_t)next;   /* Candidate integer */
        ++next;

        /* Check if m itself is sum of three squares. */
        if (!is_sum_three_squares(m)) {
            continue;
        }

        /* Primitive test:
           For every square divisor d² (d > 1) of m, ensure that m/d²
           is NOT a sum of three squares. */
        bool primitive = true;
        for (idx_t d = 2; (seq_t)d * (seq_t)d <= m; ++d) {
            seq_t sq = (seq_t)d * (seq_t)d;
            if (m % sq == 0) {
                seq_t q = m / sq;
                if (is_sum_three_squares(q)) {
                    primitive = false;
                    break;
                }
            }
        }

        if (primitive) {
            /* Grow cache if necessary. */
            ensure_capacity(cache_len + 1, &cache_cap, OEISC_VPP(&cache),
                            sizeof(seq_t));
            cache[cache_len++] = m;
        }
    }

    return cache[n - 1];
}
