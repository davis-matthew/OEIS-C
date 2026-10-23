/*
 * A167274  Triangle read by rows, = 2*A047999 - A047999^(-1); = twice
 *          Sierpinski's gasket minus the inverse of Sierpinski's gasket.
 * https://oeis.org/A167274
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = 2*A047999(n) - A047999^(-1)(n) (triangle rows read by
 *            rows)
 * Range:     u8 n <= 77, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* Compute the nth term of OEIS A167274. */
seq_t a(idx_t n)
{
    /* Find the row that contains index n. */
    idx_t r = 0;
    while ((idx_t)(r + 1) * (r + 2) / 2 <= n)
        ++r;
    idx_t row = r;
    idx_t col  = n - row * (row + 1) / 2;         /* column within the row */

    /* Width of the square matrices that will hold the triangle data. */
    size_t width = (size_t)(row + 1);
    size_t len   = width * width;

    /* Allocate temporary storage.  S contains values of the Sierpinski gasket
       (0 or 1), Inv stores the inverse matrix over the integers. */
    signed char *S   = (signed char*)malloc(len);
    int64_t    *Inv  = (int64_t*) malloc(len * sizeof(int64_t));
    if (!S || !Inv) {          /* allocation failed – return 0 as a safe fallback */
        free(S);
        free(Inv);
        return (seq_t)0;
    }

    /* Build the S matrix (Pascal triangle modulo 2). */
    for (size_t i = 0; i <= row; ++i) {
        S[i * width + 0] = 1;
        for (size_t j = 1; j <= i; ++j) {
            if (j == i)
                S[i * width + j] = 1;
            else {
                signed char left  = S[(i - 1) * width + (j - 1)];
                signed char right = S[(i - 1) * width + j];
                S[i * width + j] = left ^ right;          /* XOR gives mod‑2 addition */
            }
        }
    }

    /* Build the inverse matrix Inv of S.  It satisfies
       Σ_{k=j}^{i-1} S(i,k) * Inv(k,j) + Inv(i,j) = 0  for i > j,
       with Inv(j,j) = 1.  */
    for (size_t i = 0; i <= row; ++i) {
        for (size_t j = 0; j <= i; ++j) {
            if (i == j) {
                Inv[i * width + j] = 1;
            } else {
                int64_t sum = 0;
                for (size_t k = j; k < i; ++k) {
                    if (S[i * width + k])
                        sum += Inv[k * width + j];
                }
                Inv[i * width + j] = -sum;
            }
        }
    }

    /* Compute the required term: a = 2*S - Inv.  The result is non‑negative
       and fits into seq_t for all n up to at least 10 000. */
    signed char s_val = S[row * width + col];
    int64_t   inv_val = Inv[row * width + col];
    int64_t   a_val   = 2LL * s_val - inv_val;

    /* Clean up. */
    free(S);
    free(Inv);

    return (seq_t)a_val;
}
