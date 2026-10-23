/*
 * A125398  Number of base 10 circular n-digit numbers with adjacent digits
 *          differing by 6 or less.
 * https://oeis.org/A125398
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(0) = 1; for n ≥ 1, a(n) = trace(Aⁿ) where A[i][j] = 1 * if |i
 *            - j| ≤ 6 else 0. The matrix A is 10 × 10. This counts the *
 *            number of closed walks of length n on that graph, i.e. the
 *            number of * circular n‑digit base‑10 numbers with each adjacent
 *            pair differing by * at most 6
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

typedef struct {
    seq_t m[10][10];
} Matrix;

/* The adjacency matrix A (entries are 0 or 1). */
static const Matrix BASE_A = { .m = {
    {1,1,1,1,1,1,1,0,0,0},
    {1,1,1,1,1,1,1,1,0,0},
    {1,1,1,1,1,1,1,1,1,0},
    {1,1,1,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,1,1,1},
    {0,1,1,1,1,1,1,1,1,1},
    {0,0,1,1,1,1,1,1,1,1},
    {0,0,0,1,1,1,1,1,1,1}
} };

/* Return the identity matrix. */
static inline Matrix identity(void)
{
    Matrix I = { .m = { 0 } };
    for (int i = 0; i < 10; ++i) I.m[i][i] = 1;
    return I;
}

/* Multiply two 10×10 matrices a and b. */
static inline Matrix mul(const Matrix *a, const Matrix *b)
{
    Matrix c = { .m = { 0 } };
    for (int i = 0; i < 10; ++i) {
        for (int j = 0; j < 10; ++j) {
            seq_t s = 0;
            for (int k = 0; k < 10; ++k) {
                s += a->m[i][k] * b->m[k][j];
            }
            c.m[i][j] = s;
        }
    }
    return c;
}

/* Compute the matrix A^p using binary exponentiation. */
static Matrix mat_pow(idx_t p)
{
    Matrix result = identity();
    Matrix base   = BASE_A;
    while (p) {
        if (p & 1U) result = mul(&result, &base);
        base = mul(&base, &base);
        p >>= 1;
    }
    return result;
}

/* Compute the trace of a matrix. */
static seq_t trace(const Matrix *m)
{
    seq_t t = 0;
    for (int i = 0; i < 10; ++i) t += m->m[i][i];
    return t;
}

/* Main function required by the problem. */
seq_t a(idx_t n)
{
    if (n == 0) return 1;              /* by definition in the OEIS entry */
    Matrix p = mat_pow(n);
    return trace(&p);
}
