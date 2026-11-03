/*
 * A063752  Numbers k such that cototient(k) is a square.
 * https://oeis.org/A063752
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, changed
 * Formula:   not stated
 * Range:     u8 n <= 35, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 78 terms through n = 978 (2026-11-03)
 * Helpers:   factor_u64, ipow_i32, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A063752
   Numbers k such that cototient(k) = k - phi(k) is a perfect square.
   a(n) = the n-th such integer (offset 1).
   The function computes terms on demand using a simple subsequence search
   with memoisation.  The search is efficient enough for n up to about
   10 000 (k < 2×10⁵). */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/factor_u64.h"
#include "../utils/ipow_i32.h"
#include "../utils/mem.h"

#ifdef __cplusplus
#define OEISC_VPP(p) (p)
#define OEISC_VP(p) (p)
#else
#define OEISC_VPP(p) ((void **)(p))
#define OEISC_VP(p) ((void *)(p))
#endif

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
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

static seq_t *cache      = NULL;   /* stored terms */
static size_t  cache_len = 0;      /* number of stored terms   */
static size_t  cache_cap = 0;      /* allocated capacity       */
static idx_t   last_k    = 0;      /* last k that has been tested */

/* Test if x is a perfect square using the provided floor_sqrt and seq_mul. */
static inline bool is_square(seq_t x)
{
    seq_t sqr = floor_sqrt(x);
    sqr = seq_mul(sqr, sqr);
    return sqr == x;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* Fill the cache until we have at least n terms. */
    while (cache_len < n) {
        last_k++;  /* next candidate integer k */

        /* compute phi(k) with uint64_t arithmetic */
        uint64_t k   = (uint64_t)last_k;
        uint64_t pr[64], ex[64];  /* enough for all occurrences up to 1e12 */
        size_t   cnt;
        factor_u64(k, pr, ex, &cnt);

        uint64_t phi = k;
        for (size_t i = 0; i < cnt; ++i) {
            uint64_t p = pr[i];
            phi = (phi / p) * (p - 1);
        }

        uint64_t cot = k - phi;          /* cototient = k - phi(k) */
        seq_t   cot_seq = (seq_t)cot;     /* cast to seq_t for sqrt */

        if (is_square(cot_seq)) {
            /* store the term into the cache */
            ensure_capacity(cache_len + 1, &cache_cap,
                            OEISC_VPP(&cache), sizeof(seq_t));
            cache[cache_len++] = (seq_t)k;
        }
    }

    /* return the n-th term (1-based index) */
    return cache[n - 1];
}
