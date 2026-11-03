/*
 * A218439  a(n) = A001609(n)^2, where g.f. of A001609 is
 *          x*(1+3*x^2)/(1-x-x^3).
 * https://oeis.org/A218439
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (A001609(n))^2 where A001609 satisfies: A001609(1)=1,
 *            A001609(2)=1, A001609(3)=4, A001609(n)=A001609(n-1)+A001609(n-3)
 *            for n≥4
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 29, u64 n <= 58, u128 n <= 116,
 *            big any n
 * Validated: standard, pass, 48 terms through n = 999 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
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

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

static seq_t *gA1k = NULL;          /* cached A001609 values (1-indexed, stored 0‑based) */
static size_t gA1k_cap = 0;          /* allocated capacity           */
static size_t gA1k_computed = 0;     /* highest index computed (0‑based) */

static void _ensure_A001609(idx_t n)
{
    if (n <= (idx_t)gA1k_computed) return;

    /* Ensure we have enough space for all needed indices. */
    size_t needed_cap = (size_t)(n > 5 ? n : 5);
    ensure_capacity(needed_cap, &gA1k_cap, OEISC_VPP(&gA1k), sizeof(seq_t));

    /* Initialise the first three terms if not already done. */
    if (gA1k_computed < 3) {
        if (gA1k_cap > 0) gA1k[0] = 1;          /* A001609(1) */
        if (gA1k_cap > 1) gA1k[1] = 1;          /* A001609(2) */
        if (gA1k_cap > 2) gA1k[2] = 4;          /* A001609(3) */
        gA1k_computed = 3;
    }

    /* Compute all remaining entries up to n-1 (0‑based indices). */
    for (idx_t i = gA1k_computed; i < n; ++i) {
        /* For 1‑based index k = i+1: A001609(k) = A001609(k-1) + A001609(k-3).
           In 0‑based array: gA1k[i] = gA1k[i-1] + gA1k[i-3].            */
        gA1k[i] = gA1k[i-1] + gA1k[i-3];
    }
    gA1k_computed = n;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;        /* Undefined for n<1, guard */
    _ensure_A001609(n);
    seq_t val = gA1k[n-1];              /* A001609(n) */
    return seq_mul(val, val);           /* square */
}
