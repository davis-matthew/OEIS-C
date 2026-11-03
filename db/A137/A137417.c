/*
 * A137417  Limiting sequence when we start with positive integers (A000027)
 *          and at step n >= 1 add to the term at position n + a(n) the value
 *          a(n).
 * https://oeis.org/A137417
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 129, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 87 terms through n = 9513 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A137417:  start with a(n)=n and for each k≥1 add a(k) to a(k+a(k)) */
#include <stddef.h>                      /* for size_t */
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

static seq_t *seq_arr = NULL;            /* dynamic array of values              */
static size_t seq_cap = 0;               /* current capacity (≥ max index+1)      */
static idx_t  seq_computed = 0;          /* largest k processed so far           */

/* initialise the array when first needed */
static void init_seq(void)
{
    if (seq_arr != NULL)
        return;                          /* already initialised */

    /* at least space for indices 0..2 (since we branch on n≥1) */
    ensure_capacity(3, &seq_cap, OEISC_VPP(&seq_arr), sizeof(seq_t));
    for (size_t i = 0; i < seq_cap; ++i)
        seq_arr[i] = (seq_t)i;            /* a[i] = i initially (a[0] unused) */
}

/* ensure that index `idx` is within bounds, enlarging the array if necessary */
static void ensure_index(idx_t idx)
{
    if ((size_t)idx < seq_cap)
        return;

    size_t needed = (size_t)idx + 1;
    size_t old_cap = seq_cap;
    ensure_capacity(needed, &seq_cap, OEISC_VPP(&seq_arr), sizeof(seq_t));

    for (size_t i = old_cap; i < seq_cap; ++i)
        seq_arr[i] = (seq_t)i;            /* initialise new slots to their index */
}

/* main sequence routine requested */
seq_t a(idx_t n)
{
    init_seq();

    /* run the recurrence up to the required n */
    for (idx_t k = seq_computed + 1; k <= n; ++k) {
        seq_t  val    = seq_arr[k];
        idx_t  target = k + (idx_t)val;   /* position to update */

        ensure_index(target);
        seq_arr[target] += val;
    }
    seq_computed = n;

    return seq_arr[n];
}
