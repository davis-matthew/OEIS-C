/*
 * A210549  Triangle of coefficients of polynomials u(n,x) jointly generated
 *          with A210550; see the Formula section.
 * https://oeis.org/A210549
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the n-th element in the flattened triangle * whose rows
 *            are the coefficient vectors of polynomials u(n,x), * generated
 *            by * * u(1,x) = 1 , v(1,x) = 1 * u(n,x) = x*u(n‑1,x) +
 *            x*v(n‑1,x) + 1 * v(n,x) = u(n‑1,x) + 2*x*v(n‑1,x) + 1 * * Each
 *            row n has n coefficients, row 1: 1; row 2: 1 2; row...
 * Range:     u8 n <= 33, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* Find the row r that contains the n-th flattened term */
    idx_t r = 1;
    idx_t cum = 1;  /* cumulative elements up to row r */
    while (cum < n) {
        ++r;
        cum += r;
    }
    idx_t prevcum = cum - r;          /* elements before row r */
    idx_t k = n - prevcum - 1;        /* 0‑based column index in row r */

    /* Arrays holding the coefficient vectors of the previous row */
    seq_t *prevU = (seq_t *)malloc(sizeof(seq_t));
    seq_t *prevV = (seq_t *)malloc(sizeof(seq_t));
    if (!prevU || !prevV) return (seq_t)0;  /* allocation failure (unlikely) */
    prevU[0] = 1;
    prevV[0] = 1;

    /* Build successive rows up to row r */
    for (idx_t i = 2; i <= r; ++i) {
        /* Current row has i coefficients (indices 0 .. i-1) */
        seq_t *currU = (seq_t *)malloc(i * sizeof(seq_t));
        seq_t *currV = (seq_t *)malloc(i * sizeof(seq_t));
        if (!currU || !currV) { free(prevU); free(prevV); return (seq_t)0; }

        currU[0] = 1;
        currV[0] = prevU[0] + 1;

        for (idx_t j = 1; j < i; ++j) {
            /* U coefficient: shift previous U and V */
            currU[j] = prevU[j - 1] + prevV[j - 1];

            /* V coefficient: constant shift of previous U and a doubled shift of V */
            seq_t tmp = (seq_t)2 * prevV[j - 1];
            if (j < i - 1)  /* prevU has size i-1, so j < i-1 is safe */
                tmp += prevU[j];
            currV[j] = tmp;
        }

        /* Free previous row and advance pointers */
        free(prevU);
        free(prevV);
        prevU = currU;
        prevV = currV;
    }

    /* Desired coefficient is in the row we just computed */
    seq_t result = prevU[k];

    /* Clean up */
    free(prevU);
    free(prevV);

    return result;
}
