/*
 * A210287  Triangle of coefficients of polynomials v(n,x) jointly generated
 *          with A209999; see the Formula section.
 * https://oeis.org/A210287
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 23, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A210287 – Triangle of coefficients of the polynomials v(n,x).
 *  For each n the function returns the n‑th entry of the triangle
 *  flattened row by row; row r contains the coefficients of v(r,x)
 *  in ascending order of powers (constant term first). The recurrences
 *
 *       u(n,x) = x*u(n‑1,x) + (x+1)*v(n‑1,x) + 1,
 *       v(n,x) = u(n‑1,x) + (x+1)*v(n‑1,x) + 1,
 *
 *  with u(1,x)=v(1,x)=1, produce the triangle.
 *
 *  Complexity:  O(r²) with r = ⌈(√(8n+1)-1)/2⌉  (≈√(2n)).
 *  Works for any integer type seq_t supplied by the build.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Find the row m such that T(m‑1) < n ≤ T(m),  T(k)=k(k+1)/2. */
    idx_t t = 0;
    /* (t+1)(t+2)/2 < n  ⇒  t+1 < m  */
    while (((t + 1) * (t + 2)) / 2 < n) ++t;
    idx_t row = t + 1;                     /* current row number (≥1) */
    idx_t prevTri = (row - 1) * row / 2;   /* T(row‑1) */
    idx_t col   = n - prevTri;             /* 1‑based column in this row */
    idx_t idx   = col - 1;                 /* 0‑based index in the coefficient array */

    /* Base case: first row is just [1] */
    if (row == 1) {
        return (seq_t)1;
    }

    /* Allocate working arrays of maximal needed size (row+1 elements). */
    seq_t *uPrev = (seq_t *)malloc((row + 1) * sizeof(seq_t));
    seq_t *vPrev = (seq_t *)malloc((row + 1) * sizeof(seq_t));
    seq_t *uCur  = (seq_t *)malloc((row + 1) * sizeof(seq_t));
    seq_t *vCur  = (seq_t *)malloc((row + 1) * sizeof(seq_t));

    /* Initialize for n = 1 */
    uPrev[0] = (seq_t)1;
    vPrev[0] = (seq_t)1;
    idx_t curSize = 1;   /* current polynomial degree + 1 */

    /* Compute rows 2 … row */
    for (idx_t r = 2; r <= row; ++r) {
        idx_t newSize = curSize + 1;

        /* Zero new arrays */
        for (idx_t i = 0; i < newSize; ++i) {
            uCur[i] = (seq_t)0;
            vCur[i] = (seq_t)0;
        }

        /* Apply the recurrences */
        for (idx_t i = 0; i < curSize; ++i) {
            /* u(r) = x*u(r‑1) + (x+1)*v(r‑1) + 1 */
            uCur[i + 1] += uPrev[i];          /* x*u(r‑1) */
            uCur[i]   += vPrev[i];            /* + (x+1)*v(r‑1) constant part */
            uCur[i + 1] += vPrev[i];          /* + (x+1)*v(r‑1) x part */

            /* v(r) = u(r‑1) + (x+1)*v(r‑1) + 1 */
            vCur[i] += uPrev[i];              /* + u(r‑1) */
            vCur[i] += vPrev[i];              /* + (x+1)*v(r‑1) constant part */
            vCur[i + 1] += vPrev[i];          /* + (x+1)*v(r‑1) x part */
        }

        /* Add the constant +1 to each recurrence */
        uCur[0] += (seq_t)1;
        vCur[0] += (seq_t)1;

        /* Prepare for next iteration: current becomes previous */
        seq_t *tmp = uPrev; uPrev = uCur;  uCur = tmp;
        tmp = vPrev; vPrev = vCur;  vCur = tmp;

        curSize = newSize;
    }

    /* The desired element is in vPrev[idx] after the final iteration. */
    seq_t result = vPrev[idx];

    /* Clean up */
    free(uPrev);
    free(vPrev);
    free(uCur);   /* uCur and vCur are currently swapped copies, but freeing them
                    * is safe as they are dummy placeholders. */
    free(vCur);

    return result;
}
