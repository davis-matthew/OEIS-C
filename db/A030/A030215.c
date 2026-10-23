/*
 * A030215  Expansion of eta(q^9)*eta(q^15).
 * https://oeis.org/A030215
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A030215
   Expansion of η(q^9)*η(q^15)
   a(n) = coefficient of x^(n-1) in ∏_{k≥1}(1 - x^(9k))(1 - x^(15k))
   (with a(0)=0 since the generating function starts with an x factor) */

#include <stdint.h>
#include <stdlib.h>

/* The build supplies seq_t and idx_t; no other prototypes are needed. */

/* Return the a(n) term of the sequence. */
seq_t a(idx_t n)
{
    /* a(0) is defined to be 0. */
    if (n == 0)
        return 0;

    /* Cache generated coefficients up to the largest index seen so far. */
    static seq_t *coeff = NULL;
    static idx_t cached_size = 0;

    if (n > cached_size) {
        /* Allocate a new coefficient array. */
        size_t sz = (size_t)n;                 /* array indices 0 .. n-1 */
        seq_t *new_coeff = (seq_t *)malloc(sz * sizeof(seq_t));
        if (!new_coeff)
            return 0;                        /* in case of OOM, return 0 */

        /* Initialise all to zero, then set constant term to 1. */
        for (size_t i = 0; i < sz; ++i)
            new_coeff[i] = 0;
        new_coeff[0] = 1;

        /* Multiply by each factor (1 - x^(9k)). */
        idx_t max_k9 = (n - 1) / 9;
        for (idx_t k = 1; k <= max_k9; ++k) {
            idx_t shift = 9 * k;
            for (idx_t i = n - 1; i >= shift; --i)
                new_coeff[i] -= new_coeff[i - shift];
        }

        /* Multiply by each factor (1 - x^(15k)). */
        idx_t max_k15 = (n - 1) / 15;
        for (idx_t k = 1; k <= max_k15; ++k) {
            idx_t shift = 15 * k;
            for (idx_t i = n - 1; i >= shift; --i)
                new_coeff[i] -= new_coeff[i - shift];
        }

        /* Replace old cache if present. */
        if (coeff != NULL)
            free(coeff);
        coeff = new_coeff;
        cached_size = n;
    }

    /* a(n) equals the coefficient of x^(n-1) in the product. */
    return coeff[n - 1];
}
