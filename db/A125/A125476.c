/*
 * A125476  Number of base 19 circular n-digit numbers with adjacent digits
 *          differing by 9 or less.
 * https://oeis.org/A125476
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = trace(A^n) for n ≥ 1, where A[i][j] = 1 if |i-j| ≤ 9,
 *            a(0) = 1. (A is a 19×19 adjacency matrix of the graph on digits
 *            0…18.)
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

typedef struct { seq_t m[19][19]; } Matrix;

/* multiply two 19×19 matrices: out = a * b.
   Use 128‑bit accumulation when seq_t is ≤64‑bit (to avoid overflow for n ≤ 16). */
static inline void mat_mult(const Matrix *a, const Matrix *b, Matrix *out)
{
    for (int i = 0; i < 19; ++i) {
        for (int j = 0; j < 19; ++j) {
            if (sizeof(seq_t) <= 8) {                     /* 64‑bit or smaller */
                unsigned __int128 sum = 0;
                for (int k = 0; k < 19; ++k)
                    sum += (unsigned __int128)a->m[i][k] *
                           (unsigned __int128)b->m[k][j];
                out->m[i][j] = (seq_t)sum;
            } else {                                       /* arbitrary precision */
                seq_t sum = 0;
                for (int k = 0; k < 19; ++k)
                    sum += a->m[i][k] * b->m[k][j];
                out->m[i][j] = sum;
            }
        }
    }
}

/* Compute a(n).  The function memoises powers of the adjacency matrix A
   (A^1, A^2, A^4, …) on first invocation.  All other invocations reuse
   the cached powers. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;               /* a(0) = 1 */

    /* static cache of powers of A */
    static bool ready = false;
    static Matrix powers[15];          /* up to A^16384, enough for n ≤ 10000 */

    if (!ready) {
        /* initialise base matrix A */
        Matrix base;
        for (int i = 0; i < 19; ++i) {
            for (int j = 0; j < 19; ++j) {
                int diff = i > j ? i - j : j - i;
                base.m[i][j] = (diff <= 9) ? (seq_t)1 : (seq_t)0;
            }
        }
        powers[0] = base;
        for (idx_t k = 1; k < 15; ++k)
            mat_mult(&powers[k-1], &powers[k-1], &powers[k]);   /* power of 2 */
        ready = true;
    }

    /* build A^n by multiplying selected powers */
    Matrix result;
    /* identity matrix */
    for (int i = 0; i < 19; ++i)
        for (int j = 0; j < 19; ++j)
            result.m[i][j] = (i == j) ? (seq_t)1 : (seq_t)0;

    idx_t m = n;
    idx_t bit = 0;
    while (m > 0) {
        if (m & 1) {
            Matrix tmp;
            mat_mult(&result, &powers[bit], &tmp);
            result = tmp;
        }
        m >>= 1;
        ++bit;
    }

    /* trace of A^n */
    seq_t trace = (seq_t)0;
    for (int i = 0; i < 19; ++i)
        trace += result.m[i][i];

    return trace;
}
