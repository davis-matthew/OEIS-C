/*
 * A231730  Triangular array read by rows: row n shows the coefficients of the
 *          polynomial u(n) = c(0) + c(1)*x + ... + c(n)*x^(n) which is the
 *          numerator of the n-th convergent of the continued fraction [k, k,
 *          k, ... ], where k = x + 1/2.
 * https://oeis.org/A231730
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 21, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   seq_alloc_new_array, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A231730: coefficients of 2^n times the numerator polynomial of the
 * n‑th convergent of the continued fraction [x+1/2, x+1/2, …].
 * The sequence is obtained by reading the rows of this triangular
 * array, where row n (n ≥ 1) contains n+1 integers. */

/* The recurrence for the integer polynomials A_n = 2^n * p_{n-1} is
 *   A_0 = [1]
 *   A_1 = [1, 2]
 *   A_{n+1} = (2x + 1) * A_n + 4 * A_{n-1}      (n ≥ 1)
 *
 * For a given input n (offset 1), the element a(n) is the
 * j‑th coefficient of A_r where r is the row containing n
 * and j = n - (r-1)(r+2)/2 - 1 (0‑based).  The implementation
 * stores the polynomials A_0 … A_r in a static table and builds
 * new rows on demand. */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/seq_mul.h"
#include "../utils/seq_alloc_new_array.h"

/* Static cache of the computed polynomials A_n.  For each n we store
 * an array of length n+1 holding the coefficients of A_n. */
static seq_t **poly = NULL;    /* pointer array, size poly_cap */
static size_t   poly_cap = 0;  /* allocated capacity */
static size_t   poly_cnt = 0;  /* number of polynomials currently stored */

/* Ensure that the cache contains all polynomials A_0 … A_upto.
 * If more entries are needed, the cache is resized before adding
 * the new ones. */
static void ensure_polys(size_t upto)
{
    if (upto < poly_cnt) return;          /* already available */

    /* Expand the cache if necessary.  realloc returns void*, so cast. */
    if (poly_cap <= upto) {
        poly = (seq_t **)realloc(poly, (upto + 1) * sizeof(seq_t *));
        poly_cap = upto + 1;
    }

    /* Build the missing polynomials.  Each new polynomial is
     * independent of earlier ones once its two predecessors are
     * available. */
    for (size_t i = poly_cnt; i <= upto; ++i) {
        size_t len = i + 1;                     /* A_i has degree i   */
        seq_t *new_poly = seq_alloc_new_array(len);
        for (size_t j = 0; j < len; ++j) new_poly[j] = 0;

        if (i == 0) {                          /* A_0 = 1            */
            new_poly[0] = (seq_t)1;
        } else if (i == 1) {                   /* A_1 = 1 + 2x       */
            new_poly[0] = (seq_t)1;
            new_poly[1] = (seq_t)2;
        } else {
            seq_t *prev1 = poly[i - 1];         /* A_{i-1}            */
            seq_t *prev2 = poly[i - 2];         /* A_{i-2}            */

            /* (2x + 1) * A_{i-1} */
            for (size_t j = 0; j <= i - 1; ++j) {
                new_poly[j]     += prev1[j]; /* *1          */
                new_poly[j + 1] += seq_mul(prev1[j], (seq_t)2); /* *2x     */
            }

            /* + 4 * A_{i-2} */
            for (size_t j = 0; j <= i - 2; ++j) {
                new_poly[j] += seq_mul(prev2[j], (seq_t)4);
            }
        }

        poly[i] = new_poly;
    }

    poly_cnt = upto + 1;                    /* now we have 0…upto inclusive */
}

/* Return the n‑th element of OEIS A231730 (offset 1). */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined, but safe      */

    /* Find the row r that contains the nth term.
     * Row r has (r+1) terms and the cumulative number of terms
     * up to row r is r(r+3)/2. */
    size_t r = 1;
    idx_t cumulative = 0;
    while (true) {
        cumulative += (idx_t)(r + 1);
        if (n <= cumulative) break;
        ++r;
    }

    /* Compute the position j (0‑based) within row r. */
    idx_t before = ((idx_t)(r - 1) * (idx_t)(r + 2)) / 2;  /* terms before row r */
    idx_t j = n - before - 1;                              /* 0‑based index in row r */

    /* Build polynomials up to A_r if needed. */
    ensure_polys(r);

    /* Return the requested coefficient. */
    return poly[r][(size_t)j];
}
