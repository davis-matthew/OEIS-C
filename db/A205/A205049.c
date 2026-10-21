/*
 * A205049  Number of (n+1)X3 0..2 arrays with the number of rightwards and
 *          downwards edge increases in each 2X2 subblock equal to the number
 *          in all its horizontal and vertical neighbors.
 * https://oeis.org/A205049
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=0}^{4} Σ_{i=0}^{26} Σ_{j=0}^{26} (M_k^n)_{i,j} where
 *            M_k is the 27×27 adjacency matrix for column pairs that give a
 *            right‑raised/vertical‑raised edge increase count k for both 2×2
 *            blocks formed by the two columns. (N+1)×3 arrays ⇒ n row
 *            transitions
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 11, u64 n <= 24, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
/* seq_t and idx_t are defined by the build environment. */

#define SZ 27
#define NUM_K 5

static seq_t M_k[NUM_K][SZ][SZ];
static int matrices_built = 0;

static void build_matrices(void)
{
    int i, j, k;
    for (k = 0; k < NUM_K; ++k) {
        for (i = 0; i < SZ; ++i) {
            int a[3];
            int tmp = i;
            a[0] = tmp % 3; tmp /= 3;
            a[1] = tmp % 3; tmp /= 3;
            a[2] = tmp % 3;
            for (j = 0; j < SZ; ++j) {
                int b[3];
                int t = j;
                b[0] = t % 3; t /= 3;
                b[1] = t % 3; t /= 3;
                b[2] = t % 3;

                int top = (a[0] < b[0]) + (a[1] < b[1]) +
                          (a[0] < a[1]) + (b[0] < b[1]);
                int bottom = (a[1] < b[1]) + (a[2] < b[2]) +
                             (a[1] < a[2]) + (b[1] < b[2]);

                M_k[k][i][j] = (top == k && bottom == k) ? (seq_t)1 : (seq_t)0;
            }
        }
    }
    matrices_built = 1;
}

/* Multiply two SZ×SZ matrices: C = A * B */
static void mat_mult(seq_t A[SZ][SZ], seq_t B[SZ][SZ], seq_t C[SZ][SZ])
{
    int i, j, m;
    seq_t tmp[SZ][SZ];
    /* initialize tmp to zero */
    for (i = 0; i < SZ; ++i)
        for (j = 0; j < SZ; ++j)
            tmp[i][j] = (seq_t)0;

    for (i = 0; i < SZ; ++i) {
        for (m = 0; m < SZ; ++m) {
            if (A[i][m] == (seq_t)0) continue;
            for (j = 0; j < SZ; ++j) {
                tmp[i][j] += A[i][m] * B[m][j];
            }
        }
    }
    /* copy back to C */
    for (i = 0; i < SZ; ++i)
        for (j = 0; j < SZ; ++j)
            C[i][j] = tmp[i][j];
}

static seq_t sum_matrix(seq_t M[SZ][SZ])
{
    seq_t s = (seq_t)0;
    int i, j;
    for (i = 0; i < SZ; ++i)
        for (j = 0; j < SZ; ++j)
            s += M[i][j];
    return s;
}

/* Compute the sum of all entries of M^exp */
static seq_t power_sum(seq_t M[SZ][SZ], idx_t exp)
{
    seq_t result[SZ][SZ];
    seq_t power[SZ][SZ];
    seq_t tmp[SZ][SZ];
    int i, j;

    /* Initialize result to identity matrix, power to M */
    for (i = 0; i < SZ; ++i)
        for (j = 0; j < SZ; ++j) {
            result[i][j] = (i == j) ? (seq_t)1 : (seq_t)0;
            power[i][j] = M[i][j];
        }

    while (exp > (idx_t)0) {
        if (exp & (idx_t)1) {
            mat_mult(result, power, tmp);
            for (i = 0; i < SZ; ++i)
                for (j = 0; j < SZ; ++j)
                    result[i][j] = tmp[i][j];
        }
        mat_mult(power, power, tmp);
        for (i = 0; i < SZ; ++i)
            for (j = 0; j < SZ; ++j)
                power[i][j] = tmp[i][j];
        exp >>= 1;
    }
    return sum_matrix(result);
}

seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)0;            /* offset 1; undefined below 1 */

    if (!matrices_built) build_matrices();

    seq_t total = (seq_t)0;
    int k;
    for (k = 0; k < NUM_K; ++k) {
        total += power_sum(M_k[k], n);
    }
    return total;
}
