/*
 * A065012  Integers for which the periodic part of the continued fraction for
 *          the square root of n begins with 9.
 * https://oeis.org/A065012
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = nth integer m ≥ 1 whose continued‑fraction for sqrt(m)
 *            has first periodic term equal to 9
 * Range:     u8 none, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Validated: partial, pass, 44 terms through n = 44 (2026-11-03)
 * Helpers:   ipow_i32, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/ipow_i32.h"
#include "../utils/mem.h"

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

/* Append a value to the cached array, expanding it if necessary.  `p` is a
   pointer to the array pointer. */
static void append_candidate(seq_t **p, size_t *len, size_t *cap, seq_t val)
{
    if (*len == *cap) {
        size_t newcap = (*cap == 0) ? 16 : (*cap << 1);
        seq_t *tmp = mem_resize(seq_t, *p, newcap);
        if (!tmp) {
            exit(EXIT_FAILURE);
        }
        *p = tmp;
        *cap = newcap;
    }
    (*p)[*len] = val;
    (*len)++;
}

static seq_t *alloc_seq_array(size_t n)
{
    return mem_new(seq_t, n);
}

static bool first_quotient_is_9(seq_t m)
{
    seq_t a0 = floor_sqrt(m);
    if (a0 * a0 == m)                     /* perfect square – ignore */
        return false;

    seq_t m1 = 0;
    seq_t d1 = 1;
    seq_t a  = a0;                        /* a0 is the non repeating part */

    /* first step of the continued‑fraction algorithm, giving a1 */
    m1 = d1 * a - m1;                     /* m1 = d*a – m0 (m0 = 0) */
    d1 = (m - m1 * m1) / d1;              /* d1 = (m − m1²)/d0 (d0 = 1) */
    a  = (a0 + m1) / d1;                  /* a1 = (a0 + m1)/d1 */

    return a == 9;
}

seq_t a(idx_t n)
{
    /* Dynamic array holding found terms and state for on‑demand generation */
    static seq_t *terms   = NULL;
    static size_t len     = 0;            /* number of terms stored */
    static size_t cap     = 0;            /* allocated capacity         */
    static idx_t  next_n  = 2;            /* next integer to test      */

    /* initialise buffer on first call */
    if (!terms) {
        cap   = 64;
        terms = alloc_seq_array(cap);
    }

    /* generate terms until we have at least n of them */
    while (len < n) {
        seq_t v = (seq_t)next_n;
        if (first_quotient_is_9(v))
            append_candidate(&terms, &len, &cap, v);
        ++next_n;
    }

    return terms[n - 1];
}
