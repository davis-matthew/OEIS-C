/*
 * A221075  Simple continued fraction expansion of an infinite product.
 * https://oeis.org/A221075
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, cofr
 * Formula:   a(0) = 2, a(1) = 12, a(2k) = 1 (k ≥ 1), Let U_n = (2+√3)^n +
 *            (2-√3)^n (U_0 = 2, U_1 = 4, U_{n+1} = 4U_n - U_{n-1}). Then for
 *            k ≥ 1: a(4k-3) = U_{2k} - 2, a(4k-1) = U_{2k+1}/2 – 2
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 30, u64 n <= 64, u128 n <= 132,
 *            big any n
 * Validated: standard, pass, 60 terms through n = 2994 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/mem.h"

#ifdef __cplusplus
#define OEISC_VPP(p) (p)
#define OEISC_VP(p) (p)
#else
#define OEISC_VPP(p) ((void **)(p))
#define OEISC_VP(p) ((void *)(p))
#endif

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

static seq_t *U_cache = NULL;      /* cache of U_n values */
static size_t U_cache_len = 0;     /* number of computed values = size of valid cache */
static size_t U_cache_cap = 0;     /* allocated capacity for U_cache */

/* compute U(e) using recurrence, growing cache if necessary */
static seq_t U_value(size_t e) {
    if (e >= U_cache_len) {
        size_t needed = e + 1;
        /* ensure enough space */
        ensure_capacity(needed, &U_cache_cap, OEISC_VPP(&U_cache), sizeof(seq_t));
        /* initialize first two values if cache is empty */
        if (U_cache_len == 0) { U_cache[0] = (seq_t)2; U_cache_len = 1; }
        if (U_cache_len == 1) { U_cache[1] = (seq_t)4; U_cache_len = 2; }
        /* compute remaining values */
        for (size_t i = U_cache_len; i <= e; ++i)
            U_cache[i] = (seq_t)4 * U_cache[i-1] - U_cache[i-2];
        U_cache_len = needed;
    }
    return U_cache[e];
}

seq_t a(idx_t n) {
    /* handle the first term explicitly */
    if (n == 0) return (seq_t)2;

    /* even indices >= 2 all equal 1 */
    if ((n & 1) == 0) return (seq_t)1;

    /* odd indices */
    switch (n % 4) {
        case 1: {                  /* n = 4k-3 */
            /* k = (n+3)/4 */
            size_t k = ((size_t)n + 3) >> 2;
            size_t e = 2 * k;               /* exponent for U */
            seq_t val = U_value(e) - (seq_t)2;
            return val;
        }
        case 3: {                  /* n = 4k-1 */
            /* k = (n+1)/4 */
            size_t k = ((size_t)n + 1) >> 2;
            size_t e = 2 * k + 1;           /* exponent for U */
            seq_t val = (U_value(e) / (seq_t)2) - (seq_t)2;
            return val;
        }
        default: /* should never happen */
            return (seq_t)0;
    }
}
