/*
 * A025756  3rd-order Vatalan numbers (generalization of Catalan numbers).
 * https://oeis.org/A025756
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 22, u128 n <= 42,
 *            big any n
 * Validated: standard, pass, 40 terms through n = 158 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: 3rd-order Vatalan numbers (A025756)
   Recurrence (OEIS entry):
        n*(n-1)*a(n) - (n-1)*(19*n-36)*a(n-1) + 9*(11*n^2-51*n+60)*a(n-2)
        - 9*(3*n-7)*(3*n-8)*a(n-3) = 0
   Initial values:
        a(0)=1, a(1)=1, a(2)=4
*/

#include <stdint.h>
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
    static seq_t *cache = NULL;
    static size_t size  = 0;   /* number of terms currently cached   */
    static size_t cap   = 0;   /* allocated capacity (elements)      */

    /* Base cases */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)4;

    /* Ensure cache is large enough to contain index n */
    if ((size_t)n >= cap) {
        size_t needed = (size_t)n + 1;
        ensure_capacity(needed, &cap, OEISC_VPP(&cache), sizeof(seq_t));
    }

    /* Initialize first three terms if this is the first request */
    if (size == 0) {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)4;
        size     = 3;
    }

    /* Compute missing terms up to n */
    for (size_t k = size; k <= (size_t)n; ++k) {
#ifdef __cplusplus
        /* C++ build – assume seq_t may be an arbitrary‑precision type.
           Multiplication/division performed directly on seq_t.         */
        seq_t kseq  = (seq_t)k;
        seq_t term1 = (k - 1) * ((seq_t)(19*k - 36));
        seq_t term2 = (seq_t)9 * ((seq_t)(11*k*k - 51*k + 60));
        seq_t term3 = (seq_t)9 * ((seq_t)(3*k - 7)) * ((seq_t)(3*k - 8));

        seq_t numer = term1 * cache[k-1] - term2 * cache[k-2] + term3 * cache[k-3];
        seq_t denom = kseq * (kseq - 1);
        cache[k]    = numer / denom;
#else
        /* C build – use 128‑bit intermediate arithmetic to avoid overflow. */
        __int128 kk1   = ( __int128)k - 1;
        __int128 term1 = kk1 * (__int128)(19*k - 36);
        __int128 term2 = (__int128)9 * (__int128)(11*k*k - 51*k + 60);
        __int128 term3 = (__int128)9 * (__int128)(3*k-7) * (__int128)(3*k-8);

        __int128 numer = term1 * (__int128)cache[k-1]
                       - term2 * (__int128)cache[k-2]
                       + term3 * (__int128)cache[k-3];
        __int128 denom = (__int128)k * (__int128)(k-1);

        cache[k] = (seq_t)(numer / denom);
#endif
    }

    size = (size_t)n + 1;
    return cache[n];
}
