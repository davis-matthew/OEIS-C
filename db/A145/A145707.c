/*
 * A145707  Expansion of chi(-q) / chi(-q^10) in powers of q where chi() is a
 *          Ramanujan theta function.
 * https://oeis.org/A145707
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 45, i16 n <= 126, i32 n <= 390, i64 n <= 1309,
 *            i128 n <= 4684, big any n
 * Helpers:   seq_alloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = coefficient of x^n in ∏_{k≥1} (1 - x^{2k-1}) / (1 - x^{20k-10})
 */
#include <stddef.h>
#include <stdlib.h>

/* Using the library allocator for a tidy API */
#include "../utils/seq_alloc.h"

seq_t a(idx_t n)
{
    /* Linear-time construction of the series up to index n.
       Memoise the coefficient array so that repeated calls are fast. */
    static seq_t *coef   = NULL;   /* array of coefficients 0 ... current_cap-1 */
    static size_t current_cap = 0; /* number of slots currently allocated */

    /* Allocate (or reallocate) the array if it does not cover n+1 entries. */
    if (coef == NULL || current_cap <= (size_t)(n + 1)) {
        free(coef);                      /* free old storage if it exists */
        size_t need = (size_t)(n + 1);
        coef = (seq_t *)calloc(need, sizeof(seq_t));  /* initialise to 0 */
        current_cap = need;
        if (coef == NULL) abort();       /* allocation failed */
    } else {
        /* zero any reused entries beyond n (previous computations may have left values) */
        for (size_t i = 0; i <= (size_t)n; ++i)
            coef[i] = 0;
    }

    /* The constant term of the product is 1. */
    coef[0] = 1;

    /* Multiply by (1 - x^m) for all odd m ≤ n. */
    for (size_t m = 1; m <= (size_t)n; m += 2) {
        /* We must walk from n down to m to avoid using new values. */
        for (size_t i = (size_t)n; i >= m; --i) {
            coef[i] -= coef[i - m];
            if (i == m) break;    /* prevent unsigned underflow */
        }
    }

    /* Multiply by 1/(1 - x^m) = 1 + x^m + x^{2m} + … for all m ≡ 10 (mod 20) ≤ n. */
    for (size_t m = 10; m <= (size_t)n; m += 20) {
        for (size_t i = m; i <= (size_t)n; ++i) {
            coef[i] += coef[i - m];
        }
    }

    return coef[(size_t)n];
}
