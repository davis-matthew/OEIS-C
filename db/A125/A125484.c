/*
 * A125484  Number of base 27 circular n-digit numbers with adjacent digits
 *          differing by 9 or less.
 * https://oeis.org/A125484
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = trace(M^n) where M[i][j] = 1 if |i-j| ≤ 9 in base 27,
 *            else 0, a(0) = 1. This counts circular n‑digit base‑27 numbers
 *            whose adjacent digits differ by at most 9
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* number of digits in base 27 */
#define N 27

/* Matrix multiplication: C = A * B  (N×N matrices of seq_t) */
static void mult_matrix(const seq_t A[N][N], const seq_t B[N][N], seq_t C[N][N])
{
    /* For small seq_t use a 128‑bit intermediate to avoid overflow. */
    if (sizeof(seq_t) <= 8) {
        unsigned __int128 sum, prod;
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < N; ++j) {
                sum = 0;
                for (int k = 0; k < N; ++k) {
                    prod = (unsigned __int128)(uintptr_t)A[i][k] *
                           (uintptr_t)B[k][j];
                    sum += prod;
                }
                C[i][j] = (seq_t)sum;
            }
        }
    } else {
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < N; ++j) {
                seq_t sum = 0;
                for (int k = 0; k < N; ++k) {
                    sum = sum + A[i][k] * B[k][j];
                }
                C[i][j] = sum;
            }
        }
    }
}

/* Initialize the adjacency matrix once per program. */
static void init_adjacency(seq_t adj[N][N])
{
    static bool inited = false;
    if (inited) return;
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            int diff = i > j ? i - j : j - i;
            adj[i][j] = (diff <= 9) ? 1 : 0;
        }
    }
    inited = true;
}

/* ------------------------------------------------------------------ */
/*  Main sequence function                                               */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return 1;

    /* Adjacency matrix */
    static seq_t A[N][N];
    init_adjacency(A);

    /* Result matrix initially identity */
    seq_t R[N][N];
    for (int i = 0; i < N; ++i)
        for (int j = 0; j < N; ++j)
            R[i][j] = (i == j) ? 1 : 0;

    /* Working copy of the base matrix */
    seq_t B[N][N];
    for (int i = 0; i < N; ++i)
        for (int j = 0; j < N; ++j)
            B[i][j] = A[i][j];

    seq_t Temp[N][N];
    idx_t exp = n;
    while (exp) {
        if (exp & 1ULL) {
            mult_matrix(R, B, Temp);
            for (int i = 0; i < N; ++i)
                for (int j = 0; j < N; ++j)
                    R[i][j] = Temp[i][j];
        }
        exp >>= 1;
        if (exp) {
            mult_matrix(B, B, Temp);
            for (int i = 0; i < N; ++i)
                for (int j = 0; j < N; ++j)
                    B[i][j] = Temp[i][j];
        }
    }

    /* Trace of A^n gives the answer for n ≥ 1 */
    seq_t sum = 0;
    for (int i = 0; i < N; ++i)
        sum = sum + R[i][i];

    return sum;
}
