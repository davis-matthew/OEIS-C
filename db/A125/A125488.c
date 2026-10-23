/*
 * A125488  Number of base 31 circular n-digit numbers with adjacent digits
 *          differing by 9 or less.
 * https://oeis.org/A125488
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = trace(A^n) for n≥1, a(0)=1; A[i][j] = 1 if |i-j| ≤ 9
 *            (base‑31 digits), otherwise 0
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

/* Matrix multiplication and copy helpers */
static void mat_mul(const seq_t (*A)[31], const seq_t (*B)[31], seq_t (*C)[31]) {
    int i, j, k;
    for (i = 0; i < 31; ++i) {
        for (j = 0; j < 31; ++j) {
            seq_t sum = 0;
            for (k = 0; k < 31; ++k) {
                sum += A[i][k] * B[k][j];
            }
            C[i][j] = sum;
        }
    }
}

static void mat_copy(seq_t (*dest)[31], const seq_t (*src)[31]) {
    int i, j;
    for (i = 0; i < 31; ++i)
        for (j = 0; j < 31; ++j)
            dest[i][j] = src[i][j];
}

/* Main sequence function */
seq_t a(idx_t n) {
    static bool init = false;
    static seq_t adj[31][31];

    /* Initialise adjacency once */
    if (!init) {
        int i, j;
        for (i = 0; i < 31; ++i) {
            for (j = 0; j < 31; ++j) {
                int diff = i - j;
                if (diff < 0) diff = -diff;
                adj[i][j] = (diff <= 9) ? (seq_t)1 : (seq_t)0;
            }
        }
        init = true;
    }

    /* a(0) = 1 */
    if (n == 0) return (seq_t)1;

    seq_t res[31][31];
    seq_t base[31][31];
    seq_t tmp[31][31];
    int i, j;

    /* Identity matrix */
    for (i = 0; i < 31; ++i)
        for (j = 0; j < 31; ++j)
            res[i][j] = (i == j) ? (seq_t)1 : (seq_t)0;

    /* Base matrix = adjacency */
    mat_copy(base, adj);

    idx_t exp = n;
    while (exp > 0) {
        if (exp & 1) {
            mat_mul(res, base, tmp);
            mat_copy(res, tmp);
        }
        exp >>= 1;
        if (exp) {
            mat_mul(base, base, tmp);
            mat_copy(base, tmp);
        }
    }

    /* Trace of the resulting matrix */
    seq_t trace = 0;
    for (i = 0; i < 31; ++i)
        trace += res[i][i];
    return trace;
}
