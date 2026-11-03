/*
 * A170644  Number of reduced words of length n in Coxeter group on 11
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^49 = I.
 * https://oeis.org/A170644
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Validated: partial, pass, 18 terms through n = 17 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A170644: number of reduced words of length n in the Coxeter group
    on 11 generators S_i with relations (S_i)^2 = (S_i S_j)^49 = I.
    Recurrence derived from the rational generating function:

        G(t) = ( t^49 + 2t^48 + 2t^47 + … + 2t + 1 )
              ------------------------------------
              ( 1 – 9t – 9t^2 – … – 9t^48 + 45t^49 )

    For n ≥ 0 let P(n) = 1 if n=0, 2 if 1≤n≤48, 1 if n=49, else 0.
    Then

        a(0) = 1
        a(n) = P(n) + 9 * Σ_{k=1}^{min(48,n)} a(n–k) – 45 * a(n–49)   (n > 0)
*/

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
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
    static seq_t *cache   = NULL;   /* memoised sequence values         */
    static size_t cached  = 0;       /* number of initialised entries    */
    static size_t capacity = 0;       /* allocated array size             */

    /* Make sure the array can hold at least n+1 elements. */
    ensure_capacity((size_t)(n + 1), &capacity, OEISC_VPP(&cache),
                    sizeof(seq_t));

    /* Initialise base term if needed. */
    if (cached == 0) {
        cache[0] = (seq_t)1;
        cached = 1;
    }

    /* Compute terms from cached up to n. */
    for (idx_t i = cached; i <= n; ++i) {
        seq_t term = 0;   /* P(i) contribution */

        /* Determine the P(i) value. */
        if (i == 0)               term = (seq_t)1;
        else if (i <= 48)         term = (seq_t)2;
        else if (i == 49)         term = (seq_t)1;
        /* else term remains 0 for i > 49 */

        /* Σ_{k=1}^{min(48,i)} a(i-k) part. */
        idx_t kmax = (i < 48 ? i : 48);
        seq_t sum = 0;
        for (idx_t k = 1; k <= kmax; ++k) {
            sum += cache[i - k];
        }
        term += (seq_t)9 * sum;

        /* Subtract 45 * a(i-49) if i ≥ 49. */
        if (i >= 49) {
            term -= (seq_t)45 * cache[i - 49];
        }

        cache[i] = term;
    }

    cached = n + 1;
    return cache[n];
}
