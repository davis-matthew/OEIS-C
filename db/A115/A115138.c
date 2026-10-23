/*
 * A115138  A sequence related to Catalan numbers A000108.
 * https://oeis.org/A115138
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = (1-4*x)*(2*C(2*x)-1)/(1+x) where C is the Catalan
 *            generating function. Recurrence: let g0 = 1, gn = 2*Cn*2^n
 *            (n>0), Nn = gn - 4*g_{n-1}, a0 = 1, a_n = Nn - a_{n-1}. C_n is
 *            the nth Catalan number, computed by C_{n+1} = C_n *
 *            2*(2n+1)/(n+2)
 * Range:     i8 n <= 4, i16 n <= 6, i32 n <= 12, i64 n <= 22, i128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

static seq_t *cat   = NULL;        /* Catalan numbers C[0..] */
static seq_t *aseq  = NULL;        /* a[0..] */
static size_t max_n = 0;          /* highest index currently computed */
static seq_t pow2   = 1;           /* 2^max_n */
static seq_t g_prev = 1;           /* g_{max_n} */
static seq_t a_prev = 1;           /* a_{max_n} */

seq_t a(idx_t n)
{
    /* initialise on first call */
    if (!cat) {
        cat  = (seq_t *)malloc(sizeof(seq_t));
        aseq = (seq_t *)malloc(sizeof(seq_t));
        if (!cat || !aseq) { abort(); }
        cat[0]  = 1;
        aseq[0] = 1;
        max_n = 0;
    }

    /* if already computed, return cached value */
    if ((size_t)n <= max_n) {
        return aseq[(size_t)n];
    }

    /* expand storage */
    cat  = (seq_t *)realloc(cat,  sizeof(seq_t)*(n+1));
    aseq = (seq_t *)realloc(aseq, sizeof(seq_t)*(n+1));
    if (!cat || !aseq) { abort(); }

    /* compute new values up to n */
    for (size_t i = max_n + 1; i <= (size_t)n; ++i) {
        /* Catalan recurrence: C_i = C_{i-1} * 2*(2*(i-1)+1) / (i+1) */
        seq_t numerator   = cat[i-1] * (seq_t)2 * (seq_t)(2*(i-1)+1);
        seq_t denominator = (seq_t)(i+1);
        seq_t c_i = numerator / denominator;
        cat[i] = c_i;

        /* powers of two */
        pow2 = pow2 * (seq_t)2;          /* now pow2 = 2^i */

        /* g_i for i>0 */
        seq_t g_i = c_i * (seq_t)2 * pow2;   /* 2*C_i*2^i */

        /* N_i and a_i */
        seq_t N_i = g_i - (seq_t)4 * g_prev;
        seq_t a_i = N_i - a_prev;

        aseq[i] = a_i;

        /* update previous values */
        g_prev = g_i;
        a_prev = a_i;
    }

    max_n = n;
    return aseq[(size_t)n];
}
