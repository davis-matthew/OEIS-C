/*
 * A125447  Number of base 12 circular n-digit numbers with adjacent digits
 *          differing by 8 or less.
 * https://oeis.org/A125447
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A125447: number of base‑12 circular n‑digit numbers
   with adjacent digits differing by at most 8.
   This is equal to the trace of the n‑th power of the 12×12
   adjacency matrix A where
   A[i][j] = 1 if |i−j| ≤ 8  (including the loop i=j),
   otherwise 0.                                    */

#include <stdlib.h>     /* abs */

static void mat_mul(const seq_t a[12][12], const seq_t b[12][12], seq_t res[12][12]) {
    seq_t temp[12][12];
    for (int i = 0; i < 12; ++i) {
        for (int j = 0; j < 12; ++j) {
            seq_t sum = 0;
            for (int k = 0; k < 12; ++k) {
                sum += a[i][k] * b[k][j];
            }
            temp[i][j] = sum;
        }
    }
    /* copy temporary matrix back to result */
    for (int i = 0; i < 12; ++i)
        for (int j = 0; j < 12; ++j)
            res[i][j] = temp[i][j];
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;          /* empty number */

    /* Build adjacency matrix A */
    seq_t bin[12][12];
    for (int i = 0; i < 12; ++i) {
        for (int j = 0; j < 12; ++j) {
            bin[i][j] = (seq_t)((abs(i - j) <= 8) ? 1 : 0);
        }
    }

    /* Result matrix initialized to identity */
    seq_t res[12][12];
    for (int i = 0; i < 12; ++i)
        for (int j = 0; j < 12; ++j)
            res[i][j] = (seq_t)((i == j) ? 1 : 0);

    /* Exponentiation by squaring: compute bin^n into res */
    idx_t exp = n;
    while (exp > 0) {
        if (exp & 1) {
            seq_t tmp[12][12];
            mat_mul(res, bin, tmp);
            for (int i = 0; i < 12; ++i)
                for (int j = 0; j < 12; ++j)
                    res[i][j] = tmp[i][j];
        }
        exp >>= 1;
        if (exp == 0) break;
        seq_t tmp[12][12];
        mat_mul(bin, bin, tmp);
        for (int i = 0; i < 12; ++i)
            for (int j = 0; j < 12; ++j)
                bin[i][j] = tmp[i][j];
    }

    /* Compute trace of res */
    seq_t trace = 0;
    for (int i = 0; i < 12; ++i) trace += res[i][i];
    return trace;
}
