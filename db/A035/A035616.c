/*
 * A035616  Coefficient of x^(30-n) in the minimal polynomial for
 *          2^(1/6)+3^(1/5).
 * https://oeis.org/A035616
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, fini, full
 * Formula:   not stated
 * Range:     i8 n <= 9, i16 n <= 10, i32 n <= 30, i64 n <= 30, i128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A035616
 *  a(n) = coefficient of x^(30-n) in the minimal polynomial of
 *  α = 2^(1/6)+3^(1/5).  It is obtained as the coefficient of
 *  x^(30-n) in the characteristic polynomial of the
 *  multiplication-by-α matrix on the basis
 *
 *      { 2^(i/6) 3^(j/5) | 0 ≤ i < 6 , 0 ≤ j < 5 }   (30 basis vectors)
 *
 *  The algorithm builds this 30×30 integer matrix, computes its
 *  powers up to A^30, extracts the traces s_k = tr(A^k), and
 *  uses Newton's identities to recover the characteristic
 *  polynomial coefficients.  Indexing is OEIS‑style: a(0)=1,
 *  a(1)=0, … .
 */

#include <stddef.h>   /* for size_t, ptrdiff_t */

#define N 30          /* dimension of the matrix */

/* matrix multiplication: C = A * B   (all size N×N)   */
static void mat_mul(seq_t C[N][N], const seq_t A[N][N], const seq_t B[N][N])
{
    for (ptrdiff_t i = 0; i < N; ++i) {
        for (ptrdiff_t j = 0; j < N; ++j) {
            seq_t sum = 0;
            for (ptrdiff_t k = 0; k < N; ++k) {
                sum += A[i][k] * B[k][j];
            }
            C[i][j] = sum;
        }
    }
}

/* build the 30×30 multiplication‑by‑α matrix */
static void build_matrix(seq_t M[N][N])
{
    /* clear */
    for (ptrdiff_t i = 0; i < N; ++i)
        for (ptrdiff_t j = 0; j < N; ++j)
            M[i][j] = 0;

    /* basis ordering: idx = i*5 + j  (0≤i<6, 0≤j<5)          */
    for (ptrdiff_t i = 0; i < 6; ++i) {
        for (ptrdiff_t j = 0; j < 5; ++j) {
            ptrdiff_t col = i * 5 + j;     /* current basis vector */

            /* term from multiplying by 2^(1/6) : i+1 */
            if (i + 1 < 6) {
                ptrdiff_t row = (i + 1) * 5 + j;
                M[row][col] += 1;
            } else { /* i+1 == 6 ⇒ reduce 2^(6)=2 */
                ptrdiff_t row = (i + 1 - 6) * 5 + j;
                M[row][col] += 2;
            }

            /* term from multiplying by 3^(1/5) : j+1 */
            if (j + 1 < 5) {
                ptrdiff_t row = i * 5 + (j + 1);
                M[row][col] += 1;
            } else { /* j+1 == 5 ⇒ reduce 3^(5)=3 */
                ptrdiff_t row = i * 5 + (j + 1 - 5);
                M[row][col] += 3;
            }
        }
    }
}

/* the required sequence function */
seq_t a(idx_t n)
{
    static seq_t coeff[N + 1];
    static int initialized = 0;

    if (!initialized) {
        /* 1. Build matrix A */
        seq_t A[N][N];
        build_matrix(A);

        /* 2. Compute traces tr(A^k) for k = 1..N */
        seq_t traces[N + 1] = {0};            /* traces[0] unused */
        seq_t power[N][N];
        mat_mul(power, A, A);                /* power = A^2 after first loop */
        mat_mul(power, A, power);            /* power = A^3 ... */

        /* start with A^1 */
        seq_t cur[N][N];
        for (ptrdiff_t i = 0; i < N; ++i)
            for (ptrdiff_t j = 0; j < N; ++j)
                cur[i][j] = A[i][j];

        /* compute trace for k = 1..N */
        for (ptrdiff_t k = 1; k <= N; ++k) {
            seq_t trace = 0;
            for (ptrdiff_t i = 0; i < N; ++i)
                trace += cur[i][i];
            traces[k] = trace;

            if (k < N) {
                /* cur := cur * A for next power */
                seq_t next[N][N];
                mat_mul(next, cur, A);
                for (ptrdiff_t i = 0; i < N; ++i)
                    for (ptrdiff_t j = 0; j < N; ++j)
                        cur[i][j] = next[i][j];
            }
        }

        /* 3. Newton identities to get characteristic polynomial */
        /*   coeff[0] = 1 (for x^N) */
        coeff[0] = 1;
        for (ptrdiff_t k = 1; k <= N; ++k) {
            seq_t sum = traces[k];
            for (ptrdiff_t j = 1; j < k; ++j) {
                sum += coeff[j] * traces[k - j];
            }
            /* division by k is exact */
            coeff[k] = -(sum / (seq_t)k);
        }

        initialized = 1;
    }

    if (n <= N)
        return coeff[(size_t)n];
    return 0;
}
