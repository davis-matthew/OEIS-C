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
 * Validated: partial, pass, 24 terms through n = 23 (2026-11-03)
 * Helpers:   mem
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
#include "../utils/mem.h"

static inline void poly_mult(const seq_t *a, const seq_t *b, idx_t n, seq_t *c) {
    /* trivial O(n^2) multiplication */
    mem_set(c, 0, (n + 1) * sizeof(seq_t));
    for (idx_t i = 0; i <= n; ++i) {
        if (a[i] == 0) continue;
        for (idx_t j = 0; i + j <= n; ++j) {
            if (b[j] == 0) continue;
            c[i + j] += a[i] * b[j];
        }
    }
}

static inline void series_recip(const seq_t *q, idx_t n, seq_t *r) {
    /* q[0] must be 1 */
    r[0] = (seq_t)1;
    for (idx_t k = 1; k <= n; ++k) {
        seq_t sum = 0;
        for (idx_t i = 1; i <= k; ++i) {
            sum += q[i] * r[k - i];
        }
        r[k] = -sum;
    }
}

static inline seq_t sqrt_series(idx_t N, seq_t *s) {
    /* s[0] = 1; s^2 = 1 + 4*x - 4*x^2 */
    s[0] = (seq_t)1;
    for (idx_t k = 1; k <= N; ++k) {
        seq_t ak = 0;
        if (k == 1) ak = 4;
        else if (k == 2) ak = -4;
        seq_t sum = 0;
        for (idx_t i = 1; i < k; ++i) {
            sum += s[i] * s[k - i];
        }
        /* s[0] = 1 contributes 2*s[0]*s[k] to the sum of squares */
        s[k] = (ak - sum) / (seq_t)2;
    }
    return 0;
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;   /* early return for the first term */

    /* Allocate temporary series arrays.
       Casting the result of malloc is necessary when seq_t is a class. */
    seq_t *s  = mem_new(seq_t, (n + 1));   /* sqrt series */
    seq_t *V  = mem_new(seq_t, (n + 1));   /* (1+sqrt)/2   */
    seq_t *V2 = mem_new(seq_t, (n + 1));   /* V^2          */
    seq_t *V4 = mem_new(seq_t, (n + 1));   /* V^4          */
    seq_t *q  = mem_new(seq_t, (n + 1));   /* V^5          */
    seq_t *r  = mem_new(seq_t, (n + 1));   /* reciprocal   */

    if (!s || !V || !V2 || !V4 || !q || !r) {
        /* Allocation failed – return 0 as a safe fallback. */
        mem_free(s); mem_free(V); mem_free(V2); mem_free(V4); mem_free(q); mem_free(r);
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
    mem_free(s); mem_free(V); mem_free(V2); mem_free(V4); mem_free(q); mem_free(r);

    return result;
}
