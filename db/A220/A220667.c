/*
 * A220667  Coefficient array for the cube of Chebyshev's C polynomials.
 * https://oeis.org/A220667
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, tabf
 * Formula:   not stated
 * Range:     i8 n <= 39, i16 n <= 91, i32 n <= 91, i64 n <= 91, i128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A220667 (Coefficient array for the cube of Chebyshev's C polynomials)
 *  a(n) is the coefficient of x^m in C(r,x)^3, where r and m are extracted
 *  from n by the linear index mapping:
 *      start_r = (3*r*r - r)/2   (sum of previous row lengths)
 *      m        = n - start_r
 *  with 0 ≤ m ≤ 3*r.
 *
 *  The monic Chebyshev polynomials C_n(x) are defined by:
 *      C_0(x) = 2
 *      C_1(x) = x
 *      C_k(x) = x*C_{k-1}(x) - C_{k-2}(x)   (k ≥ 2)
 *
 *  a(n) is returned as seq_t.  The routine works for any integer type
 *  supplied by the build (int8_t…__int128 or an arbitrary‑precision class).
 */

#include <stdlib.h>
#include <stddef.h>

/* ------------------------------------------------------------------ */
/*  Helper: compute linear start index of row r  (0‑based)              */
/*  using 128‑bit arithmetic to avoid overflow                       */
/* ------------------------------------------------------------------ */
static inline unsigned __int128 row_start(idx_t r) {
    return (((unsigned __int128)3 * r * r) - r) / 2;
}

/* ------------------------------------------------------------------ */
/*                    Main entry point                                 */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* -------------------------------------------------------------- */
    /* 1. Find the row r and column m for linear index n              */
    /* -------------------------------------------------------------- */
    idx_t r = 0;
    while (1) {
        unsigned __int128 next_start = row_start(r + 1);
        if (next_start > n)
            break;
        r++;
    }

    unsigned __int128 start_val = row_start(r);
    idx_t start = (idx_t)start_val;          /* start index of row r   */
    size_t m = (size_t)(n - start);          /* offset inside the row */

    /* Guard against invalid m (outside [0,3*r]) */
    if (m > (size_t)3 * r)
        return 0;

    /* -------------------------------------------------------------- */
    /* 2. Compute C_r(x), the Chebyshev polynomial of degree r        */
    /* -------------------------------------------------------------- */
    /* We keep two previous polynomials: prev = C_{k-2}, cur = C_{k-1}  */
    size_t deg = (size_t)r;                     /* highest degree seen so far */
    seq_t *prev = (seq_t *)calloc(deg + 1, sizeof(seq_t));
    seq_t *cur  = (seq_t *)calloc(deg + 1, sizeof(seq_t));
    seq_t *next = (seq_t *)calloc(deg + 1, sizeof(seq_t));

    if (!prev || !cur || !next) {
        free(prev); free(cur); free(next);
        return 0;
    }

    /* Base polynomials */
    prev[0] = 2;                           /* C_0 = 2 */
    if (r == 0) {
        /* only C_0 needed */
    } else {
        cur[0] = 0;   /* C_1 = x, coefficient of degree 0 is 0 */
        cur[1] = 1;   /* degree 1 coefficient is 1 */

        /* Build up to C_r */
        for (idx_t k = 2; k <= r; ++k) {
            size_t len_k = (size_t)k + 1;      /* non‑zero indices up to k */

            /* Compute next = x*cur - prev */
            for (size_t i = 0; i < len_k; ++i) {
                seq_t val = 0;
                if (i >= 1)
                    val += cur[i - 1];        /* x * cur      */
                if (i <= (size_t)k - 2)
                    val -= prev[i];          /* minus prev   */
                next[i] = val;
            }
            /* Rotate buffers for next iteration */
            seq_t *tmp = prev;
            prev = cur;
            cur  = next;
            next = tmp;
        }
    }

    /* Polynomial C_r is now in 'cur' if r >= 1, otherwise in 'prev'  */
    seq_t *C = (r == 0 ? prev : cur);

    /* -------------------------------------------------------------- */
    /* 3. Form convolution Q = C * C  (degree 2*r)                     */
    /* -------------------------------------------------------------- */
    size_t qlen = 2 * (size_t)r + 1;
    seq_t *Q = (seq_t *)calloc(qlen, sizeof(seq_t));
    if (!Q) {
        free(prev); free(cur); free(next);
        return 0;
    }

    for (size_t i = 0; i <= (size_t)r; ++i) {
        if (C[i] == 0) continue;
        for (size_t j = 0; j <= (size_t)r; ++j) {
            if (C[j] == 0) continue;
            Q[i + j] += C[i] * C[j];
        }
    }

    /* -------------------------------------------------------------- */
    /* 4. Extract coefficient of x^m in C^3 = Q * C                   */
    /* -------------------------------------------------------------- */
    seq_t coeff = 0;
    size_t i_start = (m > (size_t)r) ? (m - (size_t)r) : 0;
    size_t i_end   = (m < 2 * (size_t)r) ? m : 2 * (size_t)r;

    for (size_t i = i_start; i <= i_end; ++i) {
        size_t j = m - i;        /* j ranges from 0 to r */
        if (j <= (size_t)r)
            coeff += Q[i] * C[j];
    }

    /* -------------------------------------------------------------- */
    /* 5. Clean up & return                                         */
    /* -------------------------------------------------------------- */
    free(prev); free(cur); free(next);
    free(Q);
    return coeff;
}
