/*
 * A102053  Column 2 of triangle A102051, which is the matrix inverse of
 *          triangle A101275 (number of Schroeder paths).
 * https://oeis.org/A102053
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = coeff of x^n in 32*(1-x)^2/(1+sqrt(1+4*x-4*x^2))^5
 * Range:     i8 n <= 2, i16 n <= 6, i32 n <= 13, i64 n <= 23, i128 n <= 23,
 *            big any n
 * Helpers:   poly_mult, series_recip, sqrt_series
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation expands the generating function as a power series.
   It uses only integer arithmetic on the type seq_t and thus works for
   all supplied integer sizes (including arbitrary‑precision).  All
   temporary arrays are allocated with a cast from malloc to satisfy
   the restrictions for the seq_t type.  The algorithm is
   O(n^2) in the number of terms requested; for n up to about 10000
   it runs well under the 2 s limit on typical machines. */

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/poly_mult.h"
#include "../utils/series_recip.h"
#include "../utils/sqrt_series.h"

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;   /* early return for the first term */

    /* Allocate temporary series arrays.
       Casting the result of malloc is necessary when seq_t is a class. */
    seq_t *s  = (seq_t*)malloc((n + 1) * sizeof(seq_t));   /* sqrt series */
    seq_t *V  = (seq_t*)malloc((n + 1) * sizeof(seq_t));   /* (1+sqrt)/2   */
    seq_t *V2 = (seq_t*)malloc((n + 1) * sizeof(seq_t));   /* V^2          */
    seq_t *V4 = (seq_t*)malloc((n + 1) * sizeof(seq_t));   /* V^4          */
    seq_t *q  = (seq_t*)malloc((n + 1) * sizeof(seq_t));   /* V^5          */
    seq_t *r  = (seq_t*)malloc((n + 1) * sizeof(seq_t));   /* reciprocal   */

    if (!s || !V || !V2 || !V4 || !q || !r) {
        /* Allocation failed – return 0 as a safe fallback. */
        free(s); free(V); free(V2); free(V4); free(q); free(r);
        return (seq_t)0;
    }

    /* 1. Compute sqrt series s(x) = sqrt(1+4x-4x^2) */
    sqrt_series(n, s);

    /* 2. Compute V(x) = (1 + s(x)) / 2 */
    V[0] = (s[0] + (seq_t)1) / (seq_t)2;        /* =1 */
    for (idx_t i = 1; i <= n; ++i) {
        V[i] = s[i] / (seq_t)2;
    }

    /* 3. Compute V^5 = ((V^2)^2)*V */
    poly_mult(V, V, n, V2);            /* V^2  */
    poly_mult(V2, V2, n, V4);          /* V^4  */
    poly_mult(V4, V, n, q);            /* V^5  */

    /* 4. Compute reciprocal R(x) of q(x), q[0] == 1 */
    series_recip(q, n, r);

    /* 5. Compute a(n) = coeff of x^n in (1-2x+x^2) * R(x) */
    seq_t result = r[n];
    if (n >= 1) result -= (seq_t)2 * r[n - 1];
    if (n >= 2) result += r[n - 2];

    /* 6. Clean up */
    free(s); free(V); free(V2); free(V4); free(q); free(r);

    return result;
}
