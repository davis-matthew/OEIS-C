/*
 * A234163  Number of (n+1) X (2+1) 0..4 arrays with every 2 X 2 subblock
 *          having its diagonal sum differing from its antidiagonal sum by 2
 *          (constant-stress 1 X 1 tilings).
 * https://oeis.org/A234163
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 3, u32 n <= 9, u64 n <= 20, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A234163:
   Number of (n+1) x 3 arrays with entries 0..4 such that every 2x2 subblock has
   its diagonal sum differing from its antidiagonal sum by 2.
   Computed by dynamic programming over row‑states and fast matrix exponentiation.
*/

#include <stdint.h>

#define N 125  /* 5^3 possible rows (0..4 for each of 3 columns) */

typedef struct { int a,b,c; } row_t;

/* Convert index 0..124 into row tuple (a,b,c) */
static const row_t rows[N] = {
    {0,0,0},{0,0,1},{0,0,2},{0,0,3},{0,0,4},
    {0,1,0},{0,1,1},{0,1,2},{0,1,3},{0,1,4},
    {0,2,0},{0,2,1},{0,2,2},{0,2,3},{0,2,4},
    {0,3,0},{0,3,1},{0,3,2},{0,3,3},{0,3,4},
    {0,4,0},{0,4,1},{0,4,2},{0,4,3},{0,4,4},
    {1,0,0},{1,0,1},{1,0,2},{1,0,3},{1,0,4},
    {1,1,0},{1,1,1},{1,1,2},{1,1,3},{1,1,4},
    {1,2,0},{1,2,1},{1,2,2},{1,2,3},{1,2,4},
    {1,3,0},{1,3,1},{1,3,2},{1,3,3},{1,3,4},
    {1,4,0},{1,4,1},{1,4,2},{1,4,3},{1,4,4},
    {2,0,0},{2,0,1},{2,0,2},{2,0,3},{2,0,4},
    {2,1,0},{2,1,1},{2,1,2},{2,1,3},{2,1,4},
    {2,2,0},{2,2,1},{2,2,2},{2,2,3},{2,2,4},
    {2,3,0},{2,3,1},{2,3,2},{2,3,3},{2,3,4},
    {2,4,0},{2,4,1},{2,4,2},{2,4,3},{2,4,4},
    {3,0,0},{3,0,1},{3,0,2},{3,0,3},{3,0,4},
    {3,1,0},{3,1,1},{3,1,2},{3,1,3},{3,1,4},
    {3,2,0},{3,2,1},{3,2,2},{3,2,3},{3,2,4},
    {3,3,0},{3,3,1},{3,3,2},{3,3,3},{3,3,4},
    {3,4,0},{3,4,1},{3,4,2},{3,4,3},{3,4,4},
    {4,0,0},{4,0,1},{4,0,2},{4,0,3},{4,0,4},
    {4,1,0},{4,1,1},{4,1,2},{4,1,3},{4,1,4},
    {4,2,0},{4,2,1},{4,2,2},{4,2,3},{4,2,4},
    {4,3,0},{4,3,1},{4,3,2},{4,3,3},{4,3,4},
    {4,4,0},{4,4,1},{4,4,2},{4,4,3},{4,4,4}
};

/* Check if transition from row i to row j is allowed */
static inline int valid_transition(int i, int j)
{
    const row_t *r1 = &rows[i];
    const row_t *r2 = &rows[j];
    int d1 = (r1->a + r2->b) - (r1->b + r2->a);
    if (d1 < 0) d1 = -d1;
    if (d1 != 2) return 0;
    int d2 = (r1->b + r2->c) - (r1->c + r2->b);
    if (d2 < 0) d2 = -d2;
    return d2 == 2;
}

/* Matrix multiplication: C = A * B */
static void mat_mul(const seq_t A[N][N], const seq_t B[N][N], seq_t C[N][N])
{
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++)
            C[i][j] = 0;
        for (int k = 0; k < N; k++) {
            seq_t aik = A[i][k];
            if (aik == 0) continue;
            for (int j = 0; j < N; j++) {
                C[i][j] += aik * B[k][j];
            }
        }
    }
}

/* Multiply matrix M by vector v: res = M * v */
static void mat_vec_mul(const seq_t M[N][N], const seq_t v[N], seq_t res[N])
{
    for (int i = 0; i < N; i++) {
        seq_t sum = 0;
        for (int j = 0; j < N; j++) {
            sum += M[i][j] * v[j];
        }
        res[i] = sum;
    }
}

/* Compute A234163(n) for n >= 1 */
seq_t a(idx_t n)
{
    /* Build adjacency matrix only once */
    static int built = 0;
    static seq_t adj[N][N];
    if (!built) {
        for (int i = 0; i < N; i++)
            for (int j = 0; j < N; j++)
                adj[i][j] = valid_transition(i, j) ? 1 : 0;
        built = 1;
    }

    /* Prepare base matrix for exponentiation */
    seq_t base[N][N];
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            base[i][j] = adj[i][j];

    /* Result vector initialized to all ones (v0) */
    seq_t res_vec[N];
    for (int i = 0; i < N; i++)
        res_vec[i] = 1;

    idx_t exp = n;              /* number of transitions needed */

    while (exp > 0) {
        if (exp & 1) {
            seq_t tmp[N];
            mat_vec_mul(base, res_vec, tmp);
            for (int i = 0; i < N; i++)
                res_vec[i] = tmp[i];
        }
        exp >>= 1;
        if (exp == 0) break;
        seq_t tmp_mat[N][N];
        mat_mul(base, base, tmp_mat);
        for (int i = 0; i < N; i++)
            for (int j = 0; j < N; j++)
                base[i][j] = tmp_mat[i][j];
    }

    /* Sum all entries of the final vector to get total number of arrays */
    seq_t total = 0;
    for (int i = 0; i < N; i++)
        total += res_vec[i];

    return total;
}
