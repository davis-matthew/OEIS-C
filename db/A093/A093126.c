/*
 * A093126  G.f.: A(x) = x/(1 - x - G001190(x^2)), where G001190 is the g.f.
 *          of A001190, the Wedderburn-Etherington numbers (binary rooted
 *          trees).
 * https://oeis.org/A093126
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n‑1) + Σ_{k=1}^{⌊n/2⌋} W(k) * a(n‑2k), where W(k) are
 *            the Wedderburn‑Etherington numbers. Initial values: a(1)=1,
 *            a(2)=1. W(1)=1, and for m≥2, W(m) = ( Σ_{i=1}^{m‑1} W(i)·W(m‑i)
 *            + [m even ? W(m/2) : 0] ) / 2
 * Range:     u8 n <= 11, u16 n <= 20, u32 n <= 39, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Standard headers for memory allocation and integer types. */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Static storage for the sequence values (a) and for Wedderburn‑Etherington numbers (w). */
static seq_t *a_cache = NULL;   /* a[0]..a[max_a] */
static seq_t *w_cache = NULL;   /* w[0]..w[max_w] (w[0] unused, set to 0) */
static idx_t  a_cap   = 0;       /* total allocated slots for a_cache   */
static idx_t  w_cap   = 0;       /* total allocated slots for w_cache   */
static idx_t  a_max   = 0;       /* largest n for which a[n] is known   */
static idx_t  w_max   = 0;       /* largest m for which W(m) is known   */

/* Ensure that a_cache and w_cache contain values up to the requested indices. */
static void grow_to(idx_t n)
{
    /* ----- Grow the Wedderburn‑Etherington table up to floor(n/2) ----- */
    idx_t needed_w = n / 2;                    /* largest w index needed          */
    if (needed_w > w_max)
    {
        if (w_cache == NULL)
        {
            w_cap = needed_w + 1;              /* +1 because we index from 0      */
            w_cache = (seq_t *)calloc((size_t)w_cap, sizeof(seq_t));
            w_cache[0] = 0;                    /* not used but convenient        */
            w_cache[1] = 1;                    /* W(1)=1                           */
            w_max   = 1;
        }
        else if (needed_w + 1 > w_cap)
        {
            w_cap = needed_w + 1;
            w_cache = (seq_t *)realloc(w_cache, (size_t)w_cap * sizeof(seq_t));
        }

        /* Compute new w values */
        for (idx_t m = w_max + 1; m <= needed_w; ++m)
        {
            seq_t sum = 0;
            for (idx_t i = 1; i < m; ++i)
                sum += w_cache[i] * w_cache[m - i];
            if ((m & 1U) == 0)                 /* m is even */
                sum += w_cache[m / 2];
            w_cache[m] = sum / 2;              /* division is exact               */
        }
        w_max = needed_w;
    }

    /* ----- Grow the a(n) table up to n ----- */
    if (a_cache == NULL)
    {
        a_cap = (n < 2 ? 2 : n + 1);
        a_cache = (seq_t *)calloc((size_t)a_cap, sizeof(seq_t));
        a_cache[0] = 0;
        a_cache[1] = 1;
        a_max = (n >= 1 ? 1 : 0);
    }
    else if (n + 1 > a_cap)
    {
        a_cap = n + 1;
        a_cache = (seq_t *)realloc(a_cache, (size_t)a_cap * sizeof(seq_t));
    }

    /* Compute missing a values. */
    for (idx_t i = a_max + 1; i <= n; ++i)
    {
        seq_t sum = 0;
        for (idx_t k = 1; k <= i / 2; ++k)
            sum += w_cache[k] * a_cache[i - 2 * k];
        a_cache[i] = a_cache[i - 1] + sum;
    }
    a_max = n;
}

/* -------------------------------------------------------------------------- */
/* Public function required by the test harness. */
seq_t a(idx_t n)
{
    if (n == 0)           /* not used for OEIS offset, but keep consistent */
        return 0;
    grow_to(n);
    return a_cache[n];
}
