/*
 * A209147  Triangle of coefficients of polynomials v(n,x) jointly generated
 *          with A209146; see the Formula section.
 * https://oeis.org/A209147
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 30, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence for the triangle of coefficients of v(n,x)
   (OEIS A209147)

   u(1,x) = 1                           ; v(1,x) = 1
   u(n,x) = u(n-1,x) + (x+1) * v(n-1,x)
   v(n,x) = u(n-1,x) + 2x * v(n-1,x) + 1

   a(n) is obtained by listing the coefficients of v(n,x) row by row.
   The first row contributes 1 term, the second 2 terms, the third 3, …
   (row r has r coefficients).  Thus the number of terms up to row r
   is r*(r+1)/2.  The function below computes a(n) for any n ≥ 1
   by iterating the above recurrence until the desired term is reached.
   It uses only integer arithmetic as required. */
#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == (idx_t)0)   /* safety; the sequence is defined for n≥1 */
        return (seq_t)0;

    /* initial row (n = 1) : u(1,x)=1, v(1,x)=1 */
    size_t len = 1;                      /* current number of coefficients */
    seq_t *uPrev = (seq_t *)malloc(len * sizeof(seq_t));
    seq_t *vPrev = (seq_t *)malloc(len * sizeof(seq_t));
    uPrev[0] = (seq_t)1;
    vPrev[0] = (seq_t)1;

    /* generated holds the total number of terms produced so far */
    idx_t generated = 1;                 /* (1*2)/2 */
    idx_t row = 1;                       /* current row number, 1‑based */

    /* if the requested term lies in the first row, return it immediately */
    if (n == 1) {
        seq_t res = vPrev[0];
        free(uPrev); free(vPrev);
        return res;
    }

    /* Generate subsequent rows until we have produced at least n terms */
    while (generated < n) {
        size_t newLen = len + 1;
        seq_t *uCur = (seq_t *)malloc(newLen * sizeof(seq_t));
        seq_t *vCur = (seq_t *)malloc(newLen * sizeof(seq_t));

        /* i == 0: constant term of the new polynomials */
        uCur[0] = uPrev[0] + vPrev[0];
        vCur[0] = uPrev[0] + (seq_t)1;

        /* 1 ≤ i < len : middle coefficients */
        for (size_t i = 1; i < len; ++i) {
            uCur[i] = uPrev[i] + vPrev[i] + vPrev[i - 1];
            vCur[i] = uPrev[i] + (seq_t)(2) * vPrev[i - 1];
        }

        /* i == len : highest coefficient of the new polynomials */
        uCur[newLen - 1] = vPrev[len - 1];
        vCur[newLen - 1] = (seq_t)(2) * vPrev[len - 1];

        /* Prepare for the next iteration */
        free(uPrev); free(vPrev);
        uPrev = uCur; vPrev = vCur;
        len = newLen;
        generated += (idx_t)newLen;   /* add the new row's terms */
        ++row;
    }

    /* The current row now contains the n‑th term.
       Compute the zero‑based position of that term within the row. */
    idx_t prevCount = (idx_t)row * ((idx_t)row - 1) / 2;  /* terms before this row */
    size_t pos = (size_t)(n - prevCount - (idx_t)1);       /* zero‑based index */

    seq_t result = vPrev[pos];

    free(uPrev); free(vPrev);
    return result;
}
