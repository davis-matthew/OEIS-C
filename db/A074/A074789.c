/*
 * A074789  Partial sums of usigma(n)^2: square of the sum of unitary divisors
 *          of n.
 * https://oeis.org/A074789
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=1..n} usigma(k)^2, * where usigma(k) = ∏_{p^e∥k}
 *            (1 + p^e)
 * Range:     u8 n <= 6, u16 n <= 45, u32 n <= 1866, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 56 terms through n = 9810 (2026-11-03)
 * Helpers:   ipow_idx, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/ipow_idx.h"
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

seq_t a(idx_t n)
{
    static seq_t *usigma = NULL;
    static seq_t *prefix = NULL;
    static uint64_t *spf = NULL;
    static size_t usigma_cap = 0, prefix_cap = 0, spf_cap = 0;
    static idx_t max_computed = 0;

    if (n <= max_computed) {
        return prefix[n];
    }

    /* make sure capacity is sufficient */
    ensure_capacity((size_t)(n + 1), &usigma_cap, OEISC_VPP(&usigma), sizeof(seq_t));
    ensure_capacity((size_t)(n + 1), &prefix_cap, OEISC_VPP(&prefix), sizeof(seq_t));
    ensure_capacity((size_t)(n + 1), &spf_cap, OEISC_VPP(&spf), sizeof(uint64_t));

    /* compute smallest prime factor table */
    spf[0] = 0;
    spf[1] = 1;
    for (idx_t i = 2; i <= n; ++i) {
        spf[i] = i;
    }
    for (idx_t i = 2; (idx_t)i * i <= n; ++i) {
        if (spf[i] == i) {
            for (idx_t j = i * i; j <= n; j += i) {
                if (spf[j] == j) {
                    spf[j] = i;
                }
            }
        }
    }

    /* compute usigma and prefix sums */
    prefix[0] = 0;
    usigma[1] = 1;
    prefix[1] = usigma[1] * usigma[1];

    for (idx_t k = 2; k <= n; ++k) {
        seq_t s = 1;
        idx_t v = k;
        while (v > 1) {
            uint64_t p = spf[v];
            idx_t e = 0;
            while (v % p == 0) {
                v /= p;
                ++e;
            }
            seq_t p_pow = ipow_idx((seq_t)p, e);
            seq_t factor = 1 + p_pow;
            s *= factor;
        }
        usigma[k] = s;
        prefix[k] = prefix[k - 1] + s * s;
    }

    max_computed = n;
    return prefix[n];
}
