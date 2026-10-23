/*
 * A125353  Number of base 16 circular n-digit numbers with adjacent digits
 *          differing by 4 or less.
 * https://oeis.org/A125353
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 1 (n=0), otherwise a(n)=trace(A^n). A is the 16×16
 *            adjacency matrix with A[i][j] = 1 if |i-j|≤4, else 0
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Seq_t and idx_t are defined by the build. */
#define N 16

typedef seq_t mat[N][N];

static void mat_zero(mat m) {
    for (int i = 0; i < N; ++i)
        for (int j = 0; j < N; ++j)
            m[i][j] = 0;
}

static void mat_identity(mat m) {
    mat_zero(m);
    for (int i = 0; i < N; ++i)
        m[i][i] = 1;
}

static void mat_copy(mat dst, const mat src) {
    for (int i = 0; i < N; ++i)
        for (int j = 0; j < N; ++j)
            dst[i][j] = src[i][j];
}

static void mat_mul(const mat a, const mat b, mat res) {
    mat_zero(res);
    for (int i = 0; i < N; ++i) {
        for (int k = 0; k < N; ++k) {
            seq_t aik = a[i][k];
            if (!aik) continue;
            for (int j = 0; j < N; ++j) {
                res[i][j] += aik * b[k][j];
            }
        }
    }
}

/* Build the adjacency matrix once. */
static void build_adj(mat m) {
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            int d = i > j ? i - j : j - i;
            m[i][j] = (d <= 4) ? 1 : 0;
        }
    }
}

seq_t a(idx_t n) {
    if (n == 0) return 1;  /* special case a(0)=1 */

    static mat baseAdj;
    static int adj_init = 0;
    if (!adj_init) {
        build_adj(baseAdj);
        adj_init = 1;
    }

    mat result, base, temp;
    mat_identity(result);
    mat_copy(base, baseAdj);

    idx_t exp = n;
    while (exp) {
        if (exp & 1) {
            mat_mul(result, base, temp);
            mat_copy(result, temp);
        }
        mat_mul(base, base, temp);
        mat_copy(base, temp);
        exp >>= 1;
    }

    seq_t trace = 0;
    for (int i = 0; i < N; ++i)
        trace += result[i][i];
    return trace;
}
