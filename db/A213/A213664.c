/*
 * A213664  Irregular triangle read by rows: T(n,k) is the number of
 *          dominating subsets with k vertices of the graph G(n) obtained by
 *          joining a vertex with two consecutive vertices of the cycle graph
 *          C_n (n >=3).
 * https://oeis.org/A213664
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 54, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A213664 - Irregular triangle read by rows.
    The nth term is the n‑th coefficient when the rows of the
    domination polynomials g[n] are concatenated.
    Recurrence for the polynomials (for n≥6):
        g[n] = x*g[n-1] + x*g[n-2] + x*g[n-3]
    Initial polynomials:
        g[3] = 2x+6x²+4x³+x⁴
        g[4] = 7x²+10x³+5x⁴+x⁵
        g[5] = 5x²+16x³+15x⁴+6x⁵+x⁶
    The function uses only integer arithmetic and memoises no state.
    The algorithm works for any n≥1, and runs in O(n) time with O(n)
    auxiliary memory.  */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Find the row r (n≥3) that contains the nth coefficient. */
    idx_t r = 3;                     /* start with row 3 (four coefficients) */
    idx_t total = 4;                 /* cumulative number of coefficients up to row r */
    while (total < n) {
        ++r;
        total += r + 1;               /* row r has r+1 coefficients */
    }
    idx_t total_before = total - (r + 1);  /* coefficients before row r */
    idx_t offset = n - total_before;       /* 1‑based offset within row r */

    /* Explicit initial rows.  Offset is 1‑based. */
    if (r == 3) {
        static const seq_t base3[5] = {0, 2, 6, 4, 1};
        return base3[offset];
    }
    if (r == 4) {
        static const seq_t base4[6] = {0, 0, 7, 10, 5, 1};
        return base4[offset];
    }
    if (r == 5) {
        /* Row 5: 0,5,16,15,6,1 */
        static const seq_t base5[7] = {0, 0, 5, 16, 15, 6, 1};
        return base5[offset];
    }

    /* Allocate temporary storage for three consecutive polynomials. */
    size_t sz = (size_t)(r + 2);          /* maximal degree index for g[r] */
    seq_t *a3 = (seq_t*)malloc(sz * sizeof(seq_t));
    seq_t *a2 = (seq_t*)malloc(sz * sizeof(seq_t));
    seq_t *a1 = (seq_t*)malloc(sz * sizeof(seq_t));
    seq_t *c  = (seq_t*)malloc(sz * sizeof(seq_t));

    if (!a3 || !a2 || !a1 || !c) {
        free(a3); free(a2); free(a1); free(c);
        return 0;                       /* allocation failure – unlikely */
    }
    /* Zero initialise. */
    for (size_t i = 0; i < sz; ++i) {
        a3[i] = a2[i] = a1[i] = c[i] = 0;
    }

    /* Initialise the known polynomials g[3], g[4], g[5]. */
    a3[1] = 2;  a3[2] = 6;  a3[3] = 4;  a3[4] = 1;        /* g[3] */
    a2[1] = 0;  a2[2] = 7;  a2[3] = 10; a2[4] = 5; a2[5] = 1; /* g[4] */
    a1[1] = 0;  a1[2] = 5;  a1[3] = 16; a1[4] = 15; a1[5] = 6; a1[6] = 1; /* g[5] */

    /* Pointers to keep the original allocations for freeing later. */
    seq_t *orig_a3 = a3, *orig_a2 = a2, *orig_a1 = a1, *orig_c = c;

    /* Generate polynomials up to row r.
       a3 ← g[n‑3], a2 ← g[n‑2], a1 ← g[n‑1], c ← g[n]. */
    for (idx_t i = 6; i <= r; ++i) {
        idx_t deg = i + 1;                 /* degree of g[i] */
        /* c[k] = a1[k-1] + a2[k-1] + a3[k-1]   (shifted by one degree) */
        for (idx_t k = 1; k <= deg; ++k) {
            c[k] = a1[k - 1] + a2[k - 1] + a3[k - 1];
        }
        /* Rotate references: next iteration will view c as the newest polynomial. */
        seq_t *tmp = a3;
        a3 = a2;
        a2 = a1;
        a1 = c;
        c  = tmp;
    }

    seq_t result = a1[offset];            /* a1 now holds g[r] */

    /* Free all allocated memory. */
    free(orig_a3);
    free(orig_a2);
    free(orig_a1);
    free(orig_c);

    return result;
}
