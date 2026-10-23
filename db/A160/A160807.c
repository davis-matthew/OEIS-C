/*
 * A160807  a(n) = A160799(n)/4.
 * https://oeis.org/A160807
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coeff of x^(n-2) in ∏_{i≥0} (1 + 5x^(2^i) + 7x^(2·2^i) +
 *            3x^(3·2^i)) where a(1) = 0
 * Range:     u8 n <= 10, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Coefficients of the 4‑term polynomial p(x) = 1 + 5x + 7x² + 3x³. */
static const seq_t coeffs[4] = {(seq_t)1, (seq_t)5, (seq_t)7, (seq_t)3};

/* Return the n‑th term of sequence A160807 (offset 1). */
seq_t a(idx_t n)
{
    /* For n = 1 the formula yields 0. */
    if (n <= 1)
        return 0;

    /* We need the coefficient of x^(n‑2) in the infinite product. */
    idx_t deg = n - 2;

    /* Allocate two working arrays of length deg+1. */
    seq_t *prod   = (seq_t *)calloc((size_t)(deg + 1), sizeof(seq_t));
    seq_t *next   = (seq_t *)calloc((size_t)(deg + 1), sizeof(seq_t));
    if (!prod || !next) {
        free(prod);
        free(next);
        return 0;   /* graceful fallback on allocation failure */
    }

    /* Initialise with the constant term of the product: 1. */
    prod[0] = (seq_t)1;

    /* Iterate over factors p(x^(2^i)) for 2^i ≤ deg. */
    for (idx_t i = 0; ; ++i) {
        /* Compute 2^i, detect overflow or exceeded range. */
        if (i >= 64)          /* idx_t is uint64_t; guard against shift overflow */
            break;
        idx_t base = ((idx_t)1) << i;
        if (base > deg || base == 0)
            break;

        /* Zero the next array. */
        for (idx_t k = 0; k <= deg; ++k)
            next[k] = 0;

        /* Convolve current product with the sparse polynomial p(x^base). */
        for (idx_t k = 0; k <= deg; ++k) {
            seq_t ak = prod[k];
            if (ak == 0)
                continue;
            for (int t = 0; t < 4; ++t) {
                idx_t shift = (idx_t)t * base;
                idx_t idx = k + shift;
                if (idx > deg)
                    continue;
                next[idx] += ak * coeffs[t];
            }
        }

        /* Prepare for next factor.  Swap prod and next. */
        seq_t *tmp = prod;
        prod = next;
        next = tmp;
    }

    seq_t result = prod[deg];

    free(prod);
    free(next);

    return result;
}
