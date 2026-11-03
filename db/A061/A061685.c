/*
 * A061685  Generalized Bell numbers: column 4 of A275043.
 * https://oeis.org/A061685
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/n) * Σ_{k=0}^{n-1} binomial(n,k)^4 * (n-k) * a(k) a(0)
 *            = 1 Recurrence is used; values are memoised for arbitrary‑order
 *            access
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 9, u128 n <= 14,
 *            big any n
 * Validated: standard, pass, 31 terms through n = 162 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
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

static seq_t binom_fast(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;          /* use symmetry */
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        res = res * (seq_t)(n - k + i);
        res = res / (seq_t)i;          /* exact division */
    }
    return res;
}

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static idx_t  last   = 0;   /* largest index computed */

    /* initialise cache for a(0) */
    if (cache == NULL) {
        ensure_capacity((size_t)1, &cap, OEISC_VPP(&cache), sizeof(seq_t));
        cache[0] = 1;
        last = 0;
    }

    /* compute any missing values up to n */
    if (n > last) {
        ensure_capacity((size_t)(n + 1), &cap, OEISC_VPP(&cache), sizeof(seq_t));
        for (idx_t m = last + 1; m <= n; ++m) {
            seq_t sum = 0;
            for (idx_t k = 0; k < m; ++k) {
                seq_t bin  = binom_fast(m, k);
                seq_t bin4 = bin;
                bin4 = bin4 * bin4; /* bin^2 */
                bin4 = bin4 * bin4; /* bin^4 */
                seq_t term = bin4;
                term = term * (seq_t)(m - k);
                term = term * cache[k];
                sum = sum + term;
            }
            cache[m] = sum / (seq_t)m;   /* division is exact */
        }
        last = n;
    }

    return cache[n];
}
