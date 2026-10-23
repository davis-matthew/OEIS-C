/*
 * A152978  a(n) = A139251(n+2)/4 = A152968(n+1)/2.
 * https://oeis.org/A152978
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 88, u16 n <= 88, u32 n <= 88, u64 n <= 88, u128 n <= 88,
 *            big any n
 * Helpers:   compute_product_coeffs
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence A152978 is defined by the generating function

   G(x) = (1+x) * ( Prod_{k≥1} (1 + x^(2^k-1) + 2*x^(2^k)) – 1 ) / (1+2*x),

   where G(x) = Σ_{n≥0} a(n) x^n  and a(1) is the first term at index 1.
   The following routine evaluates a(n) for arbitrary n≥1. */

#include <stdlib.h>
#include "../utils/compute_product_coeffs.h"

/* Forward declaration for the product coefficient routine. */
static void compute_product_coeffs(idx_t n, seq_t *out);

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /* out of range – harmless */

    /* Allocate array for the coefficients of the infinite product P(x). */
    seq_t *p = (seq_t *)calloc((size_t)n + 1, sizeof(seq_t));
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

    free(p);
    return curr_a;
}

/* ------------------------------------------------------------------ */
/* Computes the coefficients of the product

      P(x) = ∏_{k≥1} (1 + x^(2^k-1) + 2*x^(2^k))

   up to degree n.  The result is placed in out[0..n].                */
