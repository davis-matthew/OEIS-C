/*
 * A125367  Number of base 30 circular n-digit numbers with adjacent digits
 *          differing by 4 or less.
 * https://oeis.org/A125367
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = trace(A^n) * where A is a 30x30 adjacency matrix with
 *            A[i][j] = 1 * if |i - j| <= 4, otherwise 0. * For n = 0, a(0) =
 *            1 (empty word)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

typedef struct {
    seq_t m[30][30];
} Matrix;

static void matrix_identity(Matrix *res) {
    for (int i = 0; i < 30; ++i) {
        for (int j = 0; j < 30; ++j) {
            res->m[i][j] = (i == j) ? (seq_t)1 : (seq_t)0;
        }
    }
}

static void matrix_mult(const Matrix *a, const Matrix *b, Matrix *res) {
    Matrix temp;
    for (int i = 0; i < 30; ++i) {
        for (int j = 0; j < 30; ++j) {
            seq_t val = (seq_t)0;
            for (int k = 0; k < 30; ++k) {
                val += a->m[i][k] * b->m[k][j];
            }
            temp.m[i][j] = val;
        }
    }
    *res = temp;
}

static seq_t matrix_trace(const Matrix *m) {
    seq_t t = (seq_t)0;
    for (int i = 0; i < 30; ++i) {
        t += m->m[i][i];
    }
    return t;
}

/* Build the adjacency matrix once */
static void fill_adjacency(Matrix *m) {
    for (int i = 0; i < 30; ++i) {
        for (int j = 0; j < 30; ++j) {
            int diff = i - j;
            if (diff < 0) diff = -diff;
            m->m[i][j] = (diff <= 4) ? (seq_t)1 : (seq_t)0;
        }
    }
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;          /* a(0) = 1 */
    /* build adjacency matrix */
    Matrix adj;
    fill_adjacency(&adj);
    /* exponentiation by squaring */
    Matrix result;
    matrix_identity(&result);
    Matrix base = adj;
    idx_t exp = n;
    while (exp > 0) {
        if (exp & (idx_t)1) {
            Matrix tmp;
            matrix_mult(&result, &base, &tmp);
            result = tmp;
        }
        exp >>= 1;
        if (exp) {
            Matrix tmp;
            matrix_mult(&base, &base, &tmp);
            base = tmp;
        }
    }
    return matrix_trace(&result);
}
