/*
 * A211338  Numbers k for which the number of divisors, tau(k), is congruent
 *          to 2 modulo 3.
 * https://oeis.org/A211338
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 98, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 76 terms through n = 9962 (2026-11-03)
 * Helpers:   mem, num_divisors
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A211338: Numbers k for which tau(k) ≡ 2 (mod 3). */
/* The function returns the n‑th such number (a(1)=2). */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/num_divisors.h"
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

static seq_t   *seq   = NULL;        /* cached sequence values */
static size_t  seq_cap = 0;          /* allocated capacity of seq  */
static size_t  seq_len = 0;          /* number of stored terms      */
static uint64_t candidate = 1;       /* next integer to test        */

seq_t a(idx_t n)
{
    /*  Produce terms until we have at least n of them. */
    while (seq_len < (size_t)n)
    {
        /* test the next integer */
        candidate++;
        uint64_t d = num_divisors(candidate);
        if ((d % 3) == 2)          /* tau(candidate) ≡ 2 (mod 3) */
        {
            /* grow the cache if necessary */
            ensure_capacity(seq_len + 1, &seq_cap, OEISC_VPP(&seq), sizeof(seq_t));
            seq[seq_len++] = (seq_t)candidate;
        }
    }
    return seq[n - 1];
}
