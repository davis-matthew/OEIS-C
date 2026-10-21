/*
 * A237650  G.f. satisfies: A(x) = (1+x+x^2)^3 * A(x^2)^2.
 * https://oeis.org/A237650
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 15, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A237650
 * Generating function: A(x) = ∏_{m≥0} (1 + x^{2^m} + x^{2^{m+1}})^{3·2^m}
 *
 * Recurrence used for the implementation:
 *   a(0) = 1
 *   Define B(k) = coefficient of x^k in (1+x+x^2)^3:
 *     B = {1,3,6,7,6,3,1}   // k = 0..6
 *   Let conv[n] = Σ_{i=0..n} a[i]·a[n-i]   (convolution of a with itself)
 *   For n ≥ 0:
 *     a(n) = Σ_{k=0}^{min(6,n)} B[k] · conv[(n−k)/2]   if (n−k) is even
 *     (skip terms where n−k is odd)
 *   Here conv[(n−k)/2] equals the coefficient of x^{n−k} in A(x^2)^2.
 *
 * The algorithm computes the sequence iteratively up to the
 * requested index, storing intermediate convolution results.
 * It works for any integer type seq_t and idx_t supplied by the build system.
 */

#include <stdlib.h>

static seq_t *a_vals = NULL;      /* array of computed a(n) values      */
static seq_t *conv   = NULL;      /* convolution array conv[n] = Σ a[i]*a[n-i]   */
static size_t   capacity = 0;     /* current allocated size              */
static idx_t    max_computed = 0; /* highest n computed so far           */
static idx_t    conv_computed = 0;/* largest n for which conv[n] is known   */

static const seq_t B[7] = {
    (seq_t)1, (seq_t)3, (seq_t)6, (seq_t)7,
    (seq_t)6, (seq_t)3, (seq_t)1
};

seq_t a(idx_t n) {
    /* Ensure capacity for indices up to n */
    if (n >= capacity) {
        size_t new_cap = (size_t)n + 1;
        a_vals = (seq_t *)realloc(a_vals, new_cap * sizeof(seq_t));
        conv   = (seq_t *)realloc(conv,   new_cap * sizeof(seq_t));
        capacity = new_cap;
    }

    /* First time: initialise a(0) and conv(0) */
    if (max_computed == 0) {
        a_vals[0] = (seq_t)1;
        conv[0]   = (seq_t)1;  /* 1*1 */
        conv_computed = 0;
        max_computed = 0;
    }

    /* Extend computation up to n (inclusive) */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        /* Ensure necessary convolution values are available */
        idx_t required = i / 2;          /* floor(i/2) */
        while (conv_computed < required) {
            ++conv_computed;
            seq_t v = 0;
            for (idx_t k = 0; k <= conv_computed; ++k) {
                v += a_vals[k] * a_vals[conv_computed - k];
            }
            conv[conv_computed] = v;
        }

        /* Compute a(i) */
        seq_t sum = 0;
        idx_t maxk = (i < 6 ? i : 6);
        for (idx_t k = 0; k <= maxk; ++k) {
            if ((i - k) & 1)            /* skip odd differences */
                continue;
            idx_t j = (i - k) / 2;      /* conv index */
            sum += B[k] * conv[j];
        }
        a_vals[i] = sum;
    }

    max_computed = (n > max_computed ? n : max_computed);
    return a_vals[n];
}
