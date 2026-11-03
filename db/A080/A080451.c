/*
 * A080451  a(1) = 7, a(n) = smallest (nontrivial) multiple of a(n-1)
 *          containing n digits, a(n) not equal to 10*a(n-1).
 * https://oeis.org/A080451
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 39,
 *            big any n
 * Validated: standard, pass, 34 terms through n = 978 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A080451:
   a(1) = 7.
   For n > 1, a(n) = smallest non‑trivial multiple of a(n-1) that
   has exactly n decimal digits; the multiple 10·a(n-1) is forbidden.
   Equivalently, let k = ceil(10^(n-1)/a(n-1)); if k==10 use k=11;
   return a(n-1)·k. */

/* required helpers from the build */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* Grow a dynamic array when needed.  The pointer is passed as (void **). */
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

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
}

seq_t a(idx_t n)
{
    /* static cache of already computed terms */
    static seq_t *cache   = NULL;   /* 1‑based indexing: cache[1] ... */
    static size_t  len    = 0;      /* highest index stored so far  */
    static size_t  cap    = 0;      /* allocated capacity          */
    static seq_t  pow10_cur = (seq_t)10; /* 10^(current_index‑1)   */

    if (n == 0) return (seq_t)0;    /* undefined, safeguard */

    /* expand cache if necessary */
    if ((size_t)n > len)
    {
        /* allocate to hold indices up to n (inclusive) */
        ensure_capacity((size_t)(n + 1), &cap, (void **)&cache, sizeof(seq_t));

        /* compute missing terms */
        for (idx_t i = len + 1; i <= n; ++i)
        {
            if (i == 1)
            {
                cache[1] = (seq_t)7;
                /* pow10_cur already set to 10 for the next step */
                continue;
            }

            seq_t prev  = cache[i - 1];
            seq_t lower = pow10_cur;   /* 10^(i-1) */

            /* k = ceil(lower / prev) */
            seq_t k = lower / prev;
            if (lower % prev != (seq_t)0)
                ++k;

            if (k < (seq_t)2)
                k = (seq_t)2;   /* enforce nontrivial multiplier */

            if (k == (seq_t)10)
                k = (seq_t)11;  /* forbid 10·prev */

            cache[i] = prev * k;

            /* prepare for next iteration: lower := lower * 10 */
            pow10_cur *= (seq_t)10;
        }

        len = (size_t)n;
    }

    return cache[(size_t)n];
}
