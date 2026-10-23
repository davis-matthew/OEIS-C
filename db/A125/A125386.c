/*
 * A125386  Number of base 23 circular n-digit numbers with adjacent digits
 *          differing by 5 or less.
 * https://oeis.org/A125386
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = trace(A^n) for n ≥ 1 with A[i][j] = 1 if |i−j| ≤ 5 (mod
 *            23), otherwise 0. a(0) = 1. (The 0‑digit circular number is
 *            counted as one.)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

/* Auxiliary matrix operation: C = X × Y   (23 × 23).  Overlap of X,Y,C
   is not allowed, so the caller must use a temporary matrix if needed.   */
static void mat_mul(const seq_t X[23][23], const seq_t Y[23][23],
                    seq_t C[23][23])
{
    for (size_t i = 0; i < 23; ++i) {
        for (size_t j = 0; j < 23; ++j) {
            seq_t sum = (seq_t)0;
            for (size_t k = 0; k < 23; ++k)
                sum += X[i][k] * Y[k][j];
            C[i][j] = sum;
        }
    }
}

/* Compute the trace of a 23 × 23 matrix M.                                         */
static seq_t mat_trace(const seq_t M[23][23])
{
    seq_t sum = (seq_t)0;
    for (size_t i = 0; i < 23; ++i)
        sum += M[i][i];
    return sum;
}

/* -------------------------------------------------------------------------- */
/* Main sequence function                                                     */
/* -------------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* 1. Preparation: adjacency matrix A and traces for n = 0 … 23.    */
    static int init = 0;
    static seq_t A[23][23];          /* A[i][j] = 1 if |i−j| ≤ 5 */
    static seq_t small_traces[24];   /* small_traces[0] = a(0) … */
    if (!init) {
        /* Build adjacency matrix A */
        for (size_t i = 0; i < 23; ++i) {
            for (size_t j = 0; j < 23; ++j) {
                size_t d = (i > j) ? (i - j) : (j - i);
                A[i][j] = (d <= 5) ? ((seq_t)1) : ((seq_t)0);
            }
        }

        /* Compute traces for n = 0 … 23 */
        seq_t M[23][23];
        /* identity matrix for n = 0 */
        for (size_t i = 0; i < 23; ++i)
            for (size_t j = 0; j < 23; ++j)
                M[i][j] = (i == j) ? ((seq_t)1) : ((seq_t)0);

        small_traces[0] = (seq_t)1;          /* a(0) = 1 */
        seq_t temp[23][23];                  /* temporary for powers */

        /* M holds A^k for k = 1 … 23 */
        for (size_t i = 0; i < 23; ++i)
            for (size_t j = 0; j < 23; ++j)
                M[i][j] = A[i][j];

        for (size_t k = 1; k <= 23; ++k) {
            small_traces[k] = mat_trace(M);
            /* M = M × A   for next power */
            mat_mul(M, A, temp);
            for (size_t i = 0; i < 23; ++i)
                for (size_t j = 0; j < 23; ++j)
                    M[i][j] = temp[i][j];
        }
        init = 1;
    }

    /* 2. Handle n directly if it is 0 … 23. */
    if (n == (idx_t)0)          return small_traces[0];
    if (n <= (idx_t)23)          return small_traces[(size_t)n];

    /* 3. Compute A^n by exponentiation by squaring, then return its trace.  */
    seq_t result[23][23], base[23][23], temp[23][23];
    /* result = identity */
    for (size_t i = 0; i < 23; ++i)
        for (size_t j = 0; j < 23; ++j)
            result[i][j] = (i == j) ? ((seq_t)1) : ((seq_t)0);

    /* base = A */
    for (size_t i = 0; i < 23; ++i)
        for (size_t j = 0; j < 23; ++j)
            base[i][j] = A[i][j];

    idx_t e = n;
    while (e > (idx_t)0) {
        if (e & (idx_t)1) {
            mat_mul(result, base, temp);
            for (size_t i = 0; i < 23; ++i)
                for (size_t j = 0; j < 23; ++j)
                    result[i][j] = temp[i][j];
        }
        mat_mul(base, base, temp);
        for (size_t i = 0; i < 23; ++i)
            for (size_t j = 0; j < 23; ++j)
                base[i][j] = temp[i][j];
        e >>= (idx_t)1;
    }

    return mat_trace(result);
}
