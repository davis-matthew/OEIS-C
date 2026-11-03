/*
 * A089381  L-th order palindromes with L > 2.
 * https://oeis.org/A089381
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) is the n-th integer > 0 that eventually cycles under the
 *            rule P(m) = (m/2) if m even, else m + rev_decimal(m), with a
 *            cycle length L > 2
 * Range:     u8 none, u16 none, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Validated: partial, pass, 33 terms through n = 33 (2026-11-03)
 * Helpers:   mem, reverse_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/reverse_digits.h"
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

/* Helper: iterate P on a number */
static inline seq_t P(seq_t m)
{
    /* m % 2 == 0  => even */
    if ((m % 2) == 0)
        return m / 2;
    else
        /* odd: m + rev(m) */
        return m + reverse_digits(m);
}

/* Detect the cycle length of the orbit of start.
   Return the cycle length; 0 if the cycle length is 2 or less. */
static int cycle_len(seq_t start)
{
    seq_t tortoise = P(start);
    seq_t hare     = P(P(start));

    /* Find a repetition point. */
    while (tortoise != hare) {
        tortoise = P(tortoise);
        hare     = P(P(hare));
    }

    /* Find the start of the cycle. */
    seq_t t = start;
    seq_t h = tortoise;
    while (t != h) {
        t = P(t);
        h = P(h);
    }

    /* Count the cycle length. */
    int lambda = 1;
    seq_t next = P(t);
    while (next != t) {
        next = P(next);
        ++lambda;
    }
    return lambda;
}

/* Static state to memoise results. */
static seq_t *cache      = NULL;
static size_t cache_cap  = 0;
static size_t cache_len  = 0;
static idx_t  next_scan  = 1;

/* Return the n-th element of the sequence. */
seq_t a(idx_t n)
{
    /* Ensure we have computed at least n elements. */
    while (cache_len < n) {
        int L = cycle_len((seq_t)next_scan);
        if (L > 2) {
            /* Append next_scan to cache. */
            if (cache_len >= cache_cap) {
                /* Grow capacity at least double. */
                size_t needed = cache_cap ? cache_cap * 2 : 16;
                ensure_capacity(needed, &cache_cap, OEISC_VPP(&cache), sizeof(seq_t));
            }
            cache[cache_len++] = (seq_t)next_scan;
        }
        ++next_scan;
    }
    return cache[n - 1];
}
