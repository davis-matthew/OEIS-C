/*
 * A159409  Number of n X n arrays of squares of integers summing to 22.
 * https://oeis.org/A159409
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = coefficient of x^22 in (1 + x + x^4 + x^9 + x^16)^(n^2)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 8, u128 n <= 22,
 *            big any n
 * Helpers:   seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/seq_mul.h"

#define TARGET_SUM 22
#define SIZE (TARGET_SUM + 1)

/* Matrix structure fixed size 23×23 */
typedef struct {
    seq_t m[SIZE][SIZE];
} matrix_t;

/* Initialize identity matrix */
static void mat_identity(matrix_t *I) {
    for (int i = 0; i < SIZE; i++)
        for (int j = 0; j < SIZE; j++)
            I->m[i][j] = (i == j) ? (seq_t)1 : (seq_t)0;
}

/* Matrix multiplication: C = A * B  (size 23×23) */
static void mat_mul(const matrix_t *A, const matrix_t *B, matrix_t *C) {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            seq_t sum = (seq_t)0;
            for (int k = 0; k < SIZE; k++) {
                sum += seq_mul(A->m[i][k], B->m[k][j]);
            }
            C->m[i][j] = sum;
        }
    }
}

/* Initialize the transition matrix M (one step of adding a square value) */
static void init_transition_matrix(matrix_t *M) {
    const int S[5] = {0, 1, 4, 9, 16};
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            int diff = i - j;
            int ok = 0;
            if (diff >= 0) {
                for (int t = 0; t < 5; t++) {
                    if (S[t] == diff) { ok = 1; break; }
                }
            }
            M->m[i][j] = ok ? (seq_t)1 : (seq_t)0;
        }
    }
}

/* Main sequence function */
seq_t a(idx_t n) {
    /* Compute k = n^2 */
    uint64_t kk = (uint64_t)n * (uint64_t)n;

    /* Static one-time initialization of transition matrix */
    static matrix_t M;
    static int M_inited = 0;
    if (!M_inited) {
        init_transition_matrix(&M);
        M_inited = 1;
    }

    /* Exponentiation by squaring: compute M^kk */
    matrix_t result, base, temp;
    mat_identity(&result);
    base = M;   /* copy */

    while (kk) {
        if (kk & 1ULL) {
            mat_mul(&result, &base, &temp);
            result = temp;             /* copy matrix */
        }
        kk >>= 1;
        if (kk) {
            mat_mul(&base, &base, &temp);
            base = temp;             /* copy matrix */
        }
    }

    /* The answer is entry [TARGET_SUM][0] of M^kk */
    return result.m[TARGET_SUM][0];
}
