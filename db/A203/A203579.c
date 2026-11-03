/*
 * A203579  Exponential (or binomial) convolution of A000032 (Lucas) with
 *          itself, divided by 2.
 * https://oeis.org/A203579
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ∑_{k=0}^n binom(n,k)·L(k)·L(n−k)/2. Recurrence: a(0)=2,
 *            a(1)=2, a(2)=7, a(n)=3·a(n−1)+2·a(n−2)−4·a(n−3) (n≥3)
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 19, u64 n <= 38, u128 n <= 76,
 *            big any n
 * Validated: standard, pass, 44 terms through n = 1883 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
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
    /* static cache of already computed terms */
    static seq_t *seq = NULL;
    static size_t seq_cap = 0;   /* allocated capacity */
    static size_t seq_len = 0;   /* number of terms initialized */

    /* initialise base terms if first call */
    if (seq_len == 0) {
        size_t need = (n >= 2 ? n + 1 : 3);
        ensure_capacity(need, &seq_cap, OEISC_VPP(&seq), sizeof(seq_t));
        seq[0] = 2;
        seq[1] = 2;
        seq[2] = 7;
        seq_len = 3;
    }

    /* return if already computed */
    if ((size_t)n < seq_len) return seq[n];

    /* grow array if necessary */
    if (seq_cap <= (size_t)n)
        ensure_capacity((size_t)n + 1, &seq_cap, OEISC_VPP(&seq), sizeof(seq_t));

    /* compute missing terms using the recurrence */
    for (size_t i = seq_len; i <= (size_t)n; ++i) {
        seq[i] = 3 * seq[i - 1] + 2 * seq[i - 2] - 4 * seq[i - 3];
    }
    seq_len = (size_t)n + 1;

    return seq[n];
}
