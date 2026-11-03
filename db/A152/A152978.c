/*
 * A152978  a(n) = A139251(n+2)/4 = A152968(n+1)/2.
 * https://oeis.org/A152978
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 88, u16 n <= 88, u32 n <= 88, u64 n <= 88, u128 n <= 88,
 *            big any n
 * Validated: partial, pass, 88 terms through n = 88 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence A152978 is defined by the generating function

   G(x) = (1+x) * ( Prod_{k≥1} (1 + x^(2^k-1) + 2*x^(2^k)) – 1 ) / (1+2*x),

   where G(x) = Σ_{n≥0} a(n) x^n  and a(1) is the first term at index 1.
   The following routine evaluates a(n) for arbitrary n≥1. */

#include <stdlib.h>
#include "../utils/mem.h"

/* ------------------------------------------------------------------ */
static void compute_product_coeffs(idx_t n, seq_t *out)
{
    size_t N = (size_t)n;

    /* Two buffers for iterative convolution. */
    seq_t *cur  = mem_new(seq_t, N + 1);
    seq_t *next = mem_new(seq_t, N + 1);
    if (!cur || !next) {
        mem_free(cur); mem_free(next);
        return;
    }

    cur[0] = (seq_t)1;                      /* start with 1 */

    for (unsigned k = 1;; ++k) {
        uint64_t shift_e = ((uint64_t)1 << k) - 1;   /* 2^k-1  */
        uint64_t shift_f = ((uint64_t)1 << k);       /* 2^k    */

        if (shift_e > n) break;                 /* no more factors affect degree ≤ n */

        /* multiply cur by the factor (1 + x^shift_e + 2*x^shift_f) */
        for (size_t i = 0; i <= N; ++i) {
            seq_t val = cur[i];
            if (val == (seq_t)0) continue;

            /* constant term 1 */
            next[i] = next[i] + val;

            /* term x^(2^k-1) */
            if (i + shift_e <= N) {
                next[i + shift_e] = next[i + shift_e] + val;
            }

            /* term 2*x^(2^k) */
            if (i + shift_f <= N) {
                next[i + shift_f] = next[i + shift_f] + ((seq_t)2 * val);
            }
        }

        /* swap buffers and clear the new “next” buffer */
        seq_t *tmp = cur; cur = next; next = tmp;
        for (size_t i = 0; i <= N; ++i) next[i] = (seq_t)0;
    }

    /* Copy product coefficients into the output array. */
    for (size_t i = 0; i <= N; ++i) out[i] = cur[i];

    mem_free(cur); mem_free(next);
}

/* Forward declaration for the product coefficient routine. */
static void compute_product_coeffs(idx_t n, seq_t *out);

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /* out of range – harmless */

    /* Allocate array for the coefficients of the infinite product P(x). */
    seq_t *p = mem_new(seq_t, (size_t)n + 1);
    if (!p) return (seq_t)0;              /* allocation failure */

    compute_product_coeffs(n, p);          /* p[0..n] are now known */

    /* a[0] = p[0] – 1 (needed as a starting value). */
    seq_t prev_a = p[0];
    prev_a = prev_a - (seq_t)1;            /* a[0] */
    seq_t curr_a = 0;                       /* placeholder */

    /* Compute a[1] … a[n] using the recurrence derived from the G.F. */
    for (idx_t i = 1; i <= n; ++i)
    {
        seq_t r = p[i] + p[i-1];          /* p[n] + p[n-1] */
        if (i == 1) r = r - (seq_t)1;     /* subtract the δ_{1,1} term */
        curr_a = r - ((seq_t)2 * prev_a);
        prev_a = curr_a;
    }

    mem_free(p);
    return curr_a;
}

/* ------------------------------------------------------------------ */
/* Computes the coefficients of the product

      P(x) = ∏_{k≥1} (1 + x^(2^k-1) + 2*x^(2^k))

   up to degree n.  The result is placed in out[0..n].                */
