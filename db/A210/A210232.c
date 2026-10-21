/*
 * A210232  Triangle of coefficients of polynomials v(n,x) jointly generated
 *          with A210231; see the Formula section.
 * https://oeis.org/A210232
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the nth term of the triangle of coefficients of the
 *            polynomials v(n,x) defined by u(1,x)=v(1,x)=1,
 *            u(n,x)=x*u(n-1,x)+v(n-1,x)+1,
 *            v(n,x)=(x+1)*u(n-1,x)+x*v(n-1,x)+1. The sequence is obtained by
 *            writing, for each n, the coefficients of v(n,x) from highest to
 *            lowest degree (left to right). T...
 * Range:     u8 n <= 32, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* below offset, arbitrary */

    /* Find the row r such that r*(r+1)/2 >= n. */
    idx_t r = 1;
    while (1) {
        __int128 total = (__int128)r * (r + 1) / 2;
        if (total >= n) break;
        ++r;
    }

    /* Allocate two arrays of length r for the previous and the current
       row coefficients of u and v. */
    seq_t *prevA = (seq_t *)calloc(r, sizeof(seq_t));
    seq_t *prevB = (seq_t *)calloc(r, sizeof(seq_t));
    seq_t *currA = (seq_t *)calloc(r, sizeof(seq_t));
    seq_t *currB = (seq_t *)calloc(r, sizeof(seq_t));
    if (!prevA || !prevB || !currA || !currB) {
        /* allocation failure – return 0 */
        free(prevA); free(prevB); free(currA); free(currB);
        return (seq_t)0;
    }

    /* Initial row n = 1 */
    prevA[0] = (seq_t)1;
    prevB[0] = (seq_t)1;

    /* Build rows up to r */
    for (idx_t row = 2; row <= r; ++row) {
        /* Compute current row coefficients */
        /* A(new,0) = B(prev,0) + 1 */
        currA[0] = prevB[0] + (seq_t)1;

        /* For k >= 1 */
        for (idx_t k = 1; k < row; ++k) {
            seq_t valBprev = (k < row - 1) ? prevB[k] : (seq_t)0;
            currA[k] = prevA[k - 1] + valBprev;
        }

        /* B(new,0) = A(prev,0) + 1 */
        currB[0] = prevA[0] + (seq_t)1;

        for (idx_t k = 1; k < row; ++k) {
            seq_t valAprevk    = (k < row - 1) ? prevA[k] : (seq_t)0;
            seq_t valAprevk_1  = prevA[k - 1];
            seq_t valBprevk_1  = prevB[k - 1];
            currB[k] = valAprevk_1 + valAprevk + valBprevk_1;
        }

        /* Swap pointers for next iteration */
        seq_t *tmpA = prevA; prevA = currA; currA = tmpA;
        seq_t *tmpB = prevB; prevB = currB; currB = tmpB;
    }

    /* Determine which coefficient in row r corresponds to the global index n */
    idx_t tri_before = (r - 1) * r / 2;          /* T(r-1) */
    idx_t index_in_row = n - tri_before - 1;     /* zero based */

    seq_t result = prevB[index_in_row];

    /* Free temporary arrays */
    free(prevA); free(prevB); free(currA); free(currB);

    return result;
}
