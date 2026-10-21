/*
 * A080450  a(1) = 6, a(n) = smallest (nontrivial) multiple of a(n-1)
 *          containing n digits, a(n) not equal to 10*a(n-1). Also a(n) is not
 *          divisible by 10.
 * https://oeis.org/A080450
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) is the smallest non‑trivial multiple of a(n‑1) that * has
 *            exactly n decimal digits, is not equal to 10·a(n‑1), and is not
 *            * divisible by 10. a(1) = 6
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    static seq_t *terms = NULL;        /* array of previously computed terms */
    static idx_t len = 0;              /* highest index already computed */
    static size_t cap = 0;             /* allocated capacity of terms */

    if (n < 1) return 0;               /* out of range – not used by tests */

    /* Lazy allocation of the terms array */
    if (terms == NULL) {
        cap = 16;
        terms = (seq_t*)malloc(cap * sizeof(seq_t));
    }
    /* Ensure we have enough capacity for the requested index */
    if (cap <= n) {
        size_t new_cap = cap ? cap : 16;
        while (new_cap <= n) new_cap <<= 1;
        terms = (seq_t*)realloc(terms, new_cap * sizeof(seq_t));
        cap = new_cap;
    }

    /* Initialise the base value a(1) = 6 if we haven't done so yet */
    if (len == 0) {
        terms[1] = 6;
        len = 1;
    }

    /* Compute all missing terms up to n */
    for (idx_t i = len + 1; i <= n; ++i) {
        seq_t prev = terms[i - 1];
        seq_t pow10_prev = pow10(i - 1);    /* 10^(i-1) */
        seq_t pow10_curr = pow10(i);        /* 10^i */

        /* Ceiling division: smallest k such that k*prev >= 10^(i-1) */
        seq_t k = (pow10_prev + prev - 1) / prev;
        if (k < 2) k = 2;                  /* non‑trivial multiple only */

        seq_t value;
        for (;;) {
            value = k * prev;
            /* Stop if the product would have more than i digits */
            if (value >= pow10_curr) break;
            /* Accept if not ending in zero and not the forbidden 10×prev */
            if ((value % 10) != 0 && k != 10) {
                terms[i] = value;
                break;
            }
            ++k;
        }
    }

    len = n;
    return terms[n];
}
