/*
 * A203194  (n-1)-st elementary symmetric function of the first n terms of
 *          (1,2,6,20,70,252,...)=A000984.
 * https://oeis.org/A203194
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∑_{k=1}^{n} (∏_{j=1}^{n} C(2j‑2, j‑1)) / C(2k‑2, k‑1)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 6, u64 n <= 9, u128 n <= 12,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Here C(a,b) is the binomial coefficient binomial(a,b). */
#include <stdlib.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Allocate space for the first n central binomial coefficients.
       n ≥ 1, so the array size is at least 1. */
    seq_t *c = (seq_t*)malloc((size_t)n * sizeof(seq_t));
    if (!c) return 0;  /* Out‑of‑memory – unlikely in the test harness. */

    /* Compute C(2i, i) for i = 0 .. n‑1.  The first value (i = 0) is 1. */
    c[0] = binomial(0, 0);   /* = 1 */
    for (idx_t i = 1; i < n; ++i) {
        c[i] = binomial(2 * i, i);
    }

    /* Compute the product P = ∏_{i=0}^{n‑1} C(2i, i). */
    seq_t product = 1;
    for (idx_t i = 0; i < n; ++i) {
        product *= c[i];
    }

    /* Sum the terms product / C(2i, i). */
    seq_t result = 0;
    for (idx_t i = 0; i < n; ++i) {
        result += product / c[i];
    }

    free(c);
    return result;
}
