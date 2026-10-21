/*
 * A236740  Number of (n+1)X(2+1) 0..2 arrays with the upper median plus the
 *          lower median of every 2X2 subblock equal.
 * https://oeis.org/A236740
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) counts (n+1)×3 arrays of 0,1,2 whose upper‑median plus
 *            lower‑median of every 2×2 block is a constant. For each possible
 *            constant S=0..4 the 27×27 matrix M_S connects row pairs that
 *            have that constant at all 2×2 subblocks. The number of arrays of
 *            height n is a(n) = Σ_{S=0..4} 1ᵀ · M_Sⁿ · 1 Th...
 * Range:     u8 none, u16 n <= 4, u32 n <= 9, u64 n <= 21, u128 n <= 43,
 *            big any n
 * Helpers:   block_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/block_sum.h"

typedef struct { seq_t v[27][27]; } Matrix;

/* convert an index 0..26 to a three‑digit base‑3 row (values 0,1,2) */
static void row_digits(int idx, int out[3]) {
    out[2] = idx % 3;
    out[1] = (idx / 3) % 3;
    out[0] = idx / 9;          /* 0,1,2 */
}

/* matrix utilities ----------------------------------------------------- */
static void mat_zero(Matrix *m) {
    for (int i = 0; i < 27; ++i)
        for (int j = 0; j < 27; ++j)
            m->v[i][j] = 0;
}

static void mat_identity(Matrix *m) {
    mat_zero(m);
    for (int i = 0; i < 27; ++i)
        m->v[i][i] = 1;
}

static void mat_assign(Matrix *dst, const Matrix *src) {
    for (int i = 0; i < 27; ++i)
        for (int j = 0; j < 27; ++j)
            dst->v[i][j] = src->v[i][j];
}

static void mat_mul(const Matrix *a, const Matrix *b, Matrix *c) {
    mat_zero(c);
    for (int i = 0; i < 27; ++i) {
        for (int j = 0; j < 27; ++j) {
            seq_t sum = 0;
            for (int k = 0; k < 27; ++k)
                sum += a->v[i][k] * b->v[k][j];
            c->v[i][j] = sum;
        }
    }
}

static void mat_pow(Matrix *result, const Matrix *base, idx_t exp) {
    Matrix r, b;
    mat_identity(&r);
    mat_assign(&b, base);
    while (exp > 0) {
        if (exp & 1) {
            Matrix tmp;
            mat_mul(&r, &b, &tmp);
            mat_assign(&r, &tmp);
        }
        exp >>= 1;
        if (exp) {
            Matrix tmp;
            mat_mul(&b, &b, &tmp);
            mat_assign(&b, &tmp);
        }
    }
    mat_assign(result, &r);
}

/* adjacency matrices for sums S=0..4 ----------------------------------- */
static Matrix ms[5];
static int ms_initialized = 0;

static void init_matrices(void) {
    if (ms_initialized) return;
    int rows[27][3];
    for (int i = 0; i < 27; ++i)
        row_digits(i, rows[i]);

    for (int s = 0; s <= 4; ++s)
        mat_zero(&ms[s]);

    for (int a = 0; a < 27; ++a)
        for (int b = 0; b < 27; ++b) {
            int ls = block_sum(rows[a], rows[b], 0);
            int rs = block_sum(rows[a], rows[b], 1);
            if (ls == rs) {
                ms[ls].v[a][b] = 1;
            }
        }
    ms_initialized = 1;
}

/* --------------------------------------------------------------------- */
/* main sequence function */
seq_t a(idx_t n) {
    if (n == 0) return 0;          /* undefined offset, return 0 */
    init_matrices();

    seq_t total = 0;
    for (int s = 0; s <= 4; ++s) {
        Matrix m;
        mat_pow(&m, &ms[s], n);          /* M_s^n */

        /* sum all entries: 1ᵀ · M_sⁿ · 1 */
        seq_t sum = 0;
        for (int i = 0; i < 27; ++i)
            for (int j = 0; j < 27; ++j)
                sum += m.v[i][j];

        total += sum;
    }
    return total;
}
