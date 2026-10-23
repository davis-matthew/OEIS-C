/*
 * A191897  Coefficients of the Z(n,x) polynomials; Z(0,x) = 1, Z(1,x) = x and
 *          Z(n,x) = x*Z(n-1,x) - 2*Z(n-2,x), n >= 2.
 * https://oeis.org/A191897
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(n) is the a(row,offset) coefficient of the polynomial Z(row,x)
 *            defined by Z(0,x)=1, Z(1,x)=x, and Z(n,x) =
 *            x*Z(n-1,x)-2*Z(n-2,x) for n≥2. The sequence is obtained by
 *            reading the triangle row by row (row 0: a(0,0), row 1:
 *            a(1,0),a(1,1), …)
 * Range:     i8 n <= 50, i16 n <= 65, i32 n <= 65, i64 n <= 65, i128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static inline idx_t
row_of(idx_t n){
    idx_t r = 0;
    /* find maximal r such that r*(r+1)/2 <= n */
    while ((r + 1) * (r + 2) / 2 <= n)
        r++;
    return r;
}

seq_t a(idx_t n){
    /* Determine row and position within the row */
    idx_t row     = row_of(n);
    idx_t offset  = n - row * (row + 1) / 2;

    /* Special cases for the first two rows */
    if (row == 0) return (seq_t)1;                /* row 0 : [1] */
    if (row == 1) return (offset == 0) ? (seq_t)1 : (seq_t)0; /* row 1 : [1,0] */

    /* Allocate storage for the first two rows */
    seq_t *prev2 = (seq_t*)malloc(sizeof(seq_t) * (row - 1)); /* row 0 */
    seq_t *prev1 = (seq_t*)malloc(sizeof(seq_t) * (row));     /* row 1 */
    if (!prev2 || !prev1) return (seq_t)0; /* memory failure – unlikely in tests */

    /* initialize row 0 and row 1 */
    prev2[0] = (seq_t)1;                     /* row 0 : [1] */
    prev1[0] = (seq_t)1;  prev1[1] = (seq_t)0;/* row 1 : [1,0] */

    /* Build rows up to the requested one */
    for (idx_t i = 2; i <= row; ++i){
        seq_t *cur = (seq_t*)malloc(sizeof(seq_t) * (i + 1));
        if (!cur) { free(prev2); free(prev1); return (seq_t)0; }

        for (idx_t j = 0; j <= i; ++j){
            seq_t term1 = (j <= i-1) ? prev1[j] : 0;           /* from row i-1 */
            seq_t term2 = (j >= 2)    ? prev2[j-2] : 0;        /* from row i-2 */
            cur[j] = term1 - (seq_t)2 * term2;
        }

        /* Shift rows for next iteration; free old data */
        free(prev2);
        prev2 = prev1;
        prev1 = cur;
    }

    seq_t result = prev1[offset];
    free(prev1);
    free(prev2);
    return result;
}
