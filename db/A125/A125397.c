/*
 * A125397  Number of base 9 circular n-digit numbers with adjacent digits
 *          differing by 6 or less.
 * https://oeis.org/A125397
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(0) = 1, a(n) = trace(A^n) for n ≥ 1, where A is the 9×9
 *            adjacency matrix with A[i][j] = 1 iff |i−j| ≤ 6 (digits 0–8 in
 *            base 9)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#define N 9

/* a matrix of size N×N */
typedef struct {
    seq_t m[N][N];
} matrix_t;

/* matrix multiplication: R = A × B */
static void mat_mul(matrix_t *R, const matrix_t *A, const matrix_t *B)
{
    int i, j, k;
    for (i = 0; i < N; ++i) {
        for (j = 0; j < N; ++j) {
            seq_t sum = (seq_t)0;
            for (k = 0; k < N; ++k) {
                sum += A->m[i][k] * B->m[k][j];
            }
            R->m[i][j] = sum;
        }
    }
}

/* fast exponentiation: res = base^exp */
static void mat_pow(matrix_t *res, const matrix_t *base, idx_t exp)
{
    matrix_t result, power, temp;
    /* identity matrix */
    for (int i = 0; i < N; ++i)
        for (int j = 0; j < N; ++j)
            result.m[i][j] = (i == j) ? (seq_t)1 : (seq_t)0;
    /* copy base into power */
    for (int i = 0; i < N; ++i)
        for (int j = 0; j < N; ++j)
            power.m[i][j] = base->m[i][j];

    while (exp > 0) {
        if (exp & ((idx_t)1))
            mat_mul(&temp, &result, &power), result = temp;
        exp >>= 1;
        if (exp == 0) break;
        mat_mul(&temp, &power, &power), power = temp;
    }
    *res = result;
}

/* trace of a matrix */
static seq_t mat_trace(const matrix_t *M)
{
    seq_t sum = (seq_t)0;
    for (int i = 0; i < N; ++i)
        sum += M->m[i][i];
    return sum;
}

/* The OEIS sequence function */
seq_t a(idx_t n)
{
    if (n == 0) /* a(0) = 1 */
        return (seq_t)1;

    /* Build adjacency matrix A with digits 0–8 */
    matrix_t A;
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            int diff = i > j ? i - j : j - i;
            A.m[i][j] = (diff <= 6) ? (seq_t)1 : (seq_t)0;
        }
    }

    matrix_t P;
    mat_pow(&P, &A, n);        /* P = A^n */
    return mat_trace(&P);      /* return trace(A^n) */
}
