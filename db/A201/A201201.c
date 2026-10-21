/*
 * A201201  Coefficient triangle for the monic associated Laguerre-Sonin(e)
 *          polynomials of order one for parameter alpha=1.
 * https://oeis.org/A201201
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, tabl
 * Formula:   a(n) enumerates the coefficients of the monic associated
 *            Laguerre‑Sinon(e) polynomials of order one with parameter alpha
 *            = 1, written row‑wise so that the constant term of La_n comes
 *            first. The recurrence used is La_n = (x - 2*(n+1)) * La_{n-1} -
 *            n*(n+1) * La_{n-2}, with La_{-1} = 0, La_0 = 1. Th...
 * Range:     i8 n <= 9, i16 n <= 20, i32 n <= 43, i64 n <= 43, i128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>   /* for size_t and types used below */

seq_t a(idx_t n) {
    /* static data used to store all computed rows of the triangle */
    static seq_t *data = NULL;      /* flattened coefficient array */
    static size_t capacity = 0;     /* allocated capacity of data */
    static size_t size = 0;         /* number of coefficients stored */
    static idx_t rowsComputed = 0;  /* last row index that has been computed */

    /* initialise on first call */
    if (!data) {
        capacity = 16;
        data = (seq_t *)malloc(capacity * sizeof(seq_t));
        if (!data) exit(1);
        data[0] = 1;          /* La_0 = 1 */
        size = 1;
    }

    /* determine the row r containing index n:
       r satisfies r*(r+1)/2 <= n < (r+1)*(r+2)/2
    */
    idx_t r = 0;
    while ( (idx_t)((r + 1) * (r + 2) / 2) <= n )
        ++r;

    /* compute rows up to r if not already done */
    while (rowsComputed < r) {
        idx_t next     = rowsComputed + 1;       /* new row index */
        idx_t lenPrev1 = next;                   /* length of row next-1 (n = next-1 => length = next) */
        idx_t lenPrev2 = (next >= 2) ? next - 1 : 0;  /* length of row next-2 */

        /* grow storage if needed */
        size_t needed = size + (next + 1);
        if (needed > capacity) {
            size_t newCap = capacity;
            while (newCap < needed) newCap <<= 1;
            seq_t *tmp = (seq_t *)realloc(data, newCap * sizeof(seq_t));
            if (!tmp) exit(1);
            data = tmp;
            capacity = newCap;
        }

        /* indices of the previous rows in the flattened array */
        idx_t startPrev1 = (next - 1) * next / 2;          /* row next-1 */
        idx_t startPrev2 = (next >= 2) ? ((next - 2) * (next - 1) / 2) : 0; /* row next-2 */

        /* build the new row */
        for (idx_t c = 0; c <= next; ++c) {
            seq_t term1 = 0;   /* coefficient from x * La_{next-1} */
            seq_t term2 = 0;   /* coefficient from -2*(next+1) * La_{next-1} */
            seq_t term3 = 0;   /* coefficient from -next*(next+1) * La_{next-2} */

            /* from x * La_{next-1} */
            if (c > 0)
                term1 = data[startPrev1 + c - 1];

            /* from -2*(next+1) * La_{next-1} */
            if (c < lenPrev1) {
                seq_t coeffPrev1_c = data[startPrev1 + c];
                term2 = coeffPrev1_c * ((seq_t)(-2) * (seq_t)(next + 1));
            }

            /* from -next*(next+1) * La_{next-2} */
            if (next >= 2 && c < lenPrev2) {
                seq_t coeffPrev2_c = data[startPrev2 + c];
                seq_t factor3 = -((seq_t)next * (seq_t)(next + 1));
                term3 = coeffPrev2_c * factor3;
            }

            data[size + c] = term1 + term2 + term3;
        }

        size += next + 1;
        rowsComputed = next;
    }

    /* retrieve the requested coefficient */
    idx_t startRow = r * (r + 1) / 2;
    idx_t offset   = n - startRow;
    return data[startRow + offset];
}
