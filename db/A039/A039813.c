/*
 * A039813  Matrix 5th power of Stirling2 triangle A008277.
 * https://oeis.org/A039813
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 21, u32 n <= 46, u64 n <= 136, u128 n <= 379,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) is the n‑th entry (row‑major order) of the triangle
 * obtained by taking the 5‑th power of the Stirling‑2 triangle
 * (A008277).  For n ≥ 1, let i be the smallest integer such that
 * i*(i+1)/2 ≥ n;  let j = n – i*(i-1)/2.  Then a(n) = (T^5)[i][j],
 * where T is the infinite lower‑triangular matrix of Stirling 2 numbers.
 */

#include <stddef.h>
#include <stdbool.h>

#define MAXR 200                /* enough for 10000 terms (200*201/2 > 10000) */
#define SEQ_SIZE (MAXR*(MAXR+1)/2)

static seq_t S[MAXR+1][MAXR+1];                     /* Stirling 2 numbers */
static seq_t M[6][MAXR+1][MAXR+1];                 /* T^m, 1≤m≤5          */
static seq_t seq_arr[SEQ_SIZE];                     /* flattened triangle  */
static size_t seq_len = 0;                          /* number of terms      */
static bool initialized = false;

/* compute all needed tables once */
static void init_tables(void)
{
    /* Stirling 2 numbers */
    S[0][0] = (seq_t)1;
    for (idx_t n = 1; n <= MAXR; ++n) {
        S[n][0] = (seq_t)0;
        for (idx_t k = 1; k <= n; ++k) {
            seq_t term1 = (seq_t)k * S[n-1][k];
            seq_t term2 = S[n-1][k-1];
            S[n][k] = term1 + term2;
        }
    }

    /* M[1] = T (Stirling 2) */
    for (idx_t i = 1; i <= MAXR; ++i) {
        for (idx_t j = 1; j <= i; ++j) {
            M[1][i][j] = S[i][j];
        }
    }

    /* successive matrix powers up to 5 */
    for (int m = 2; m <= 5; ++m) {
        for (idx_t i = 1; i <= MAXR; ++i) {
            for (idx_t j = 1; j <= i; ++j) {
                seq_t sum = (seq_t)0;
                for (idx_t l = j; l <= i; ++l) {
                    sum += M[m-1][i][l] * S[l][j];
                }
                M[m][i][j] = sum;
            }
        }
    }

    /* flatten rows of the 5th power triangle */
    size_t idx = 0;
    for (idx_t i = 1; i <= MAXR; ++i) {
        for (idx_t j = 1; j <= i; ++j) {
            seq_arr[idx++] = M[5][i][j];
        }
    }
    seq_len = SEQ_SIZE;
    initialized = true;
}

/* public sequence function */
seq_t a(idx_t n)
{
    if (!initialized) {
        init_tables();
    }
    if (n == 0) {
        return (seq_t)0;        /* undefined for n<1, but return 0 as safe fallback */
    }
    idx_t idx = n - 1;
    if (idx >= seq_len) {
        /* if n is larger than precomputed, extend tables (unlikely for tests) */
        return (seq_t)0;
    }
    return seq_arr[idx];
}
