/*
 * A125369  Number of base 32 circular n-digit numbers with adjacent digits
 *          differing by 4 or less.
 * https://oeis.org/A125369
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = trace(A^n) for n ≥ 1, a(0) = 1, where A[i][j] = 1 if |i –
 *            j| ≤ 4 (mod 32), otherwise 0
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 10, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

/* Multiply matrices of size 32 × 32
   a, b, and res are pointers to the first element of a 32-element array of
   32-element arrays of seq_t.  All operations are performed with the
   type seq_t. */
static void matmul(const seq_t a[32][32], const seq_t b[32][32],
                   seq_t res[32][32]) {
    for (idx_t i = 0; i < 32; ++i) {
        for (idx_t j = 0; j < 32; ++j) {
            seq_t sum = 0;
            for (idx_t k = 0; k < 32; ++k) {
                sum += a[i][k] * b[k][j];
            }
            res[i][j] = sum;
        }
    }
}

seq_t a(idx_t n) {
    if (n == 0)
        return (seq_t)1;

    /* Lazy initialization of the adjacency matrix.  */
    static seq_t base[32][32];
    static bool base_inited = false;
    if (!base_inited) {
        for (idx_t i = 0; i < 32; ++i) {
            for (idx_t j = 0; j < 32; ++j) {
                seq_t diff = (i > j) ? (seq_t)(i - j) : (seq_t)(j - i);
                base[i][j] = (diff <= (seq_t)4) ? (seq_t)1 : (seq_t)0;
            }
        }
        base_inited = true;
    }

    /* R = I, A = base */
    seq_t R[32][32];
    seq_t A[32][32];
    seq_t temp[32][32];

    for (idx_t i = 0; i < 32; ++i) {
        for (idx_t j = 0; j < 32; ++j) {
            R[i][j] = (i == j) ? (seq_t)1 : (seq_t)0;
            A[i][j] = base[i][j];
        }
    }

    idx_t exp = n;
    while (exp > 0) {
        if (exp & 1) {
            matmul(R, A, temp);
            /* Copy temp → R */
            for (idx_t i = 0; i < 32; ++i)
                for (idx_t j = 0; j < 32; ++j)
                    R[i][j] = temp[i][j];
        }
        matmul(A, A, temp);
        /* Copy temp → A */
        for (idx_t i = 0; i < 32; ++i)
            for (idx_t j = 0; j < 32; ++j)
                A[i][j] = temp[i][j];

        exp >>= 1;
    }

    /* Sum the diagonal entries ⇒ trace(A^n) */
    seq_t sum = 0;
    for (idx_t i = 0; i < 32; ++i)
        sum += R[i][i];

    return sum;
}
