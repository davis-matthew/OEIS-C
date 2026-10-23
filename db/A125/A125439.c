/*
 * A125439  Number of base 27 circular n-digit numbers with adjacent digits
 *          differing by 7 or less.
 * https://oeis.org/A125439
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(0)=1; for n≥1, a(n) = trace(A^n), where A is the 27×27
 *            adjacency matrix A[i][j] = 1 if |i−j|≤7, else 0
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

/* Matrix size – number of digits in base 27 */
#define LIM 27

/* adjacency matrix */
static seq_t A[LIM][LIM];
static bool A_built = false;

/* build adjacency matrix once */
static void build_A(void)
{
    if (A_built) return;
    for (size_t i = 0; i < LIM; ++i) {
        for (size_t j = 0; j < LIM; ++j) {
            size_t diff = (i > j) ? (i - j) : (j - i);
            A[i][j] = (diff <= 7) ? (seq_t)1 : (seq_t)0;
        }
    }
    A_built = true;
}

/* multiplies matrices X and Y (27×27) into Z:
     Z = X * Y
   all operations are performed in seq_t (no overflow checks). */
static void mat_mul(seq_t Z[LIM][LIM], const seq_t X[LIM][LIM],
                          const seq_t Y[LIM][LIM])
{
    seq_t T[LIM][LIM] = {0};
    for (size_t i = 0; i < LIM; ++i) {
        for (size_t k = 0; k < LIM; ++k) {
            if (X[i][k] == 0) continue;
            seq_t aik = X[i][k];
            for (size_t j = 0; j < LIM; ++j) {
                T[i][j] += aik * Y[k][j];
            }
        }
    }
    for (size_t i = 0; i < LIM; ++i) {
        for (size_t j = 0; j < LIM; ++j) {
            Z[i][j] = T[i][j];
        }
    }
}

/* identity matrix into M */
static void mat_identity(seq_t M[LIM][LIM])
{
    for (size_t i = 0; i < LIM; ++i) {
        for (size_t j = 0; j < LIM; ++j) {
            M[i][j] = (i == j) ? (seq_t)1 : (seq_t)0;
        }
    }
}

/* sum of diagonal of M */
static seq_t mat_trace(const seq_t M[LIM][LIM])
{
    seq_t s = 0;
    for (size_t i = 0; i < LIM; ++i) {
        s += M[i][i];
    }
    return s;
}

/* main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;   /* a(0) = 1 */

    build_A();   /* ensure adjacency matrix is ready */

    /* exponentiation by squaring: compute A^n and its trace */
    seq_t result[LIM][LIM];
    seq_t base[LIM][LIM];
    mat_identity(result);
    for (size_t i = 0; i < LIM; ++i) {
        for (size_t j = 0; j < LIM; ++j) {
            base[i][j] = A[i][j];
        }
    }

    idx_t exp = n;
    while (exp > 0) {
        if (exp & 1ULL) {
            seq_t tmp[LIM][LIM];
            mat_mul(tmp, result, base);
            for (size_t i = 0; i < LIM; ++i)
                for (size_t j = 0; j < LIM; ++j)
                    result[i][j] = tmp[i][j];
        }
        exp >>= 1;
        if (exp == 0) break;
        seq_t tmp[LIM][LIM];
        mat_mul(tmp, base, base);
        for (size_t i = 0; i < LIM; ++i)
            for (size_t j = 0; j < LIM; ++j)
                base[i][j] = tmp[i][j];
    }

    return mat_trace(result);
}
