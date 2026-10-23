/*
 * A125373  Number of base 10 circular n-digit numbers with adjacent digits
 *          differing by 5 or less.
 * https://oeis.org/A125373
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(0) = 1, a(n) = trace(Aⁿ) for n≥1 * where A is the 10×10
 *            adjacency matrix with * A[i][j] = 1 if |i−j| ≤ 5, otherwise 0
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   mat_mul_out
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Include standard headers needed for idx_t/seq_t operations */
#include <stddef.h>
#include "../utils/mat_mul_out.h"

/* INTERNAL: matrix multiplication returning the result in `out` */

/* Main sequence function */
seq_t a(idx_t n) {
    /* a(0) is defined as 1 */
    if (n == 0) return 1;

    /* static adjacency matrix A */
    static const seq_t ADJ[10][10] = {
        {1,1,1,1,1,1,0,0,0,0},
        {1,1,1,1,1,1,1,0,0,0},
        {1,1,1,1,1,1,1,1,0,0},
        {1,1,1,1,1,1,1,1,1,0},
        {1,1,1,1,1,1,1,1,1,1},
        {1,1,1,1,1,1,1,1,1,1},
        {0,1,1,1,1,1,1,1,1,1},
        {0,0,1,1,1,1,1,1,1,1},
        {0,0,0,1,1,1,1,1,1,1},
        {0,0,0,0,1,1,1,1,1,1}
    };

    /* Matrices for exponentiation */
    seq_t res[10][10];   /* result matrix, starts as identity */
    seq_t powm[10][10];  /* current power of adjacency, starts as A */

    /* Initialize identity matrix */
    for (idx_t i = 0; i < 10; ++i)
        for (idx_t j = 0; j < 10; ++j)
            res[i][j] = (i == j) ? 1 : 0;

    /* Copy adjacency matrix into powm */
    for (idx_t i = 0; i < 10; ++i)
        for (idx_t j = 0; j < 10; ++j)
            powm[i][j] = ADJ[i][j];

    idx_t exp = n;
    while (exp) {
        if (exp & 1) {
            mat_mul_out(res, res, powm);   /* res = res * powm */
        }
        exp >>= 1;
        if (exp) {
            mat_mul_out(powm, powm, powm); /* powm = powm * powm */
        }
    }

    /* Compute trace of res */
    seq_t sum = 0;
    for (idx_t i = 0; i < 10; ++i)
        sum += res[i][i];

    return sum;
}
