/*
 * A125448  Number of base 13 circular n-digit numbers with adjacent digits
 *          differing by 8 or less.
 * https://oeis.org/A125448
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = trace(A^n), A[i][j] = 1 iff |i-j| <= 8 (digits 0..12)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* seq_t and idx_t are defined by the build. */
static const seq_t BASE_DIGITS[13] = {0,1,2,3,4,5,6,7,8,9,10,11,12};

/* adjacency matrix for base‑13 digits */
static seq_t base_mat[13][13];

/* helper: matrix multiplication C = A * B  (13×13) */
static void mat_mult(const seq_t A[13][13], const seq_t B[13][13],
                     seq_t C[13][13]) {
    for (int i = 0; i < 13; ++i) {
        for (int j = 0; j < 13; ++j) {
            seq_t sum = 0;
            for (int k = 0; k < 13; ++k) {
                sum += A[i][k] * B[k][j];
            }
            C[i][j] = sum;
        }
    }
}

/* helper: copy matrix dst = src  (13×13) */
static void mat_copy(seq_t dst[13][13], const seq_t src[13][13]) {
    for (int i = 0; i < 13; ++i) {
        for (int j = 0; j < 13; ++j) {
            dst[i][j] = src[i][j];
        }
    }
}

/* helper: set matrix to identity */
static void mat_identity(seq_t M[13][13]) {
    for (int i = 0; i < 13; ++i) {
        for (int j = 0; j < 13; ++j) {
            M[i][j] = (i == j) ? (seq_t)1 : (seq_t)0;
        }
    }
}

/* main sequence function */
seq_t a(idx_t n)
{
    /* Initialise adjacency matrix lazily */
    static bool init_done = false;
    if (!init_done) {
        for (int i = 0; i < 13; ++i) {
            for (int j = 0; j < 13; ++j) {
                seq_t diff = (BASE_DIGITS[i] > BASE_DIGITS[j]) ?
                              (seq_t)(BASE_DIGITS[i] - BASE_DIGITS[j]) :
                              (seq_t)(BASE_DIGITS[j] - BASE_DIGITS[i]);
                base_mat[i][j] = (diff <= 8) ? (seq_t)1 : (seq_t)0;
            }
        }
        init_done = true;
    }

    if (n == 0) return (seq_t)1;          /* base case: a(0)=1 */

    /* binary exponentiation of the adjacency matrix */
    seq_t cur[13][13];   /* current power of A (will be squared each loop) */
    seq_t res[13][13];   /* result matrix, starts as identity */

    mat_copy(cur, base_mat);
    mat_identity(res);

    idx_t exp = n;
    seq_t tmp[13][13];

    while (exp) {
        if (exp & (idx_t)1) {
            mat_mult(res, cur, tmp);
            mat_copy(res, tmp);
        }
        exp >>= 1;
        if (exp) {   /* avoid unnecessary squaring at the end */
            mat_mult(cur, cur, tmp);
            mat_copy(cur, tmp);
        }
    }

    /* compute trace of res */
    seq_t result = 0;
    for (int i = 0; i < 13; ++i) {
        result += res[i][i];
    }
    return result;
}
