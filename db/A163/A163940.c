/*
 * A163940  Triangle related to the divergent series 1^m*1! - 2^m*2! + 3^m*3!
 *          - 4^m*4! + ... for m >= -1.
 * https://oeis.org/A163940
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) is the element at the flattened index n of a triangle *
 *            T(n,k) defined by * T(n,k) = S(n, n-k) + (n-k) * Σ_{j=0}^{k-1}
 *            (-1)^j * S(n, n+1+j-k) * j! * where S(n,p) are Stirling numbers
 *            of the second kind and j! is the factorial. * The sequence lists
 *            the rows of this triangle in row‑major order
 * Range:     u8 n <= 30, u16 n <= 59, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* maximum row needed for n up to 10000 (≈141) – a small safety margin */
#define MAX_N 200

static seq_t S[MAX_N + 1][MAX_N + 1];
static seq_t fact[MAX_N + 1];
static bool initialized = false;

/* initialise Stirling numbers (second kind) and factorials up to MAX_N */
static void init_tables(void) {
    if (initialized) return;
    S[0][0] = 1;
    fact[0] = 1;
    for (idx_t i = 1; i <= MAX_N; ++i) {
        fact[i] = fact[i - 1] * (seq_t)i;
        S[i][0] = 0;
        for (idx_t j = 1; j <= i; ++j) {
            S[i][j] = S[i - 1][j - 1] + (seq_t)j * S[i - 1][j];
        }
    }
    initialized = true;
}

seq_t a(idx_t n) {
    init_tables();

    /* determine row and column from the flattened index n */
    idx_t row = 0;
    /* number of terms up to and including row-1 is row*(row+1)/2 */
    while ((row + 1) * (row + 2) / 2 <= n) ++row;
    idx_t prev_terms = row * (row + 1) / 2;
    idx_t col = n - prev_terms;  /* 0 <= col <= row */

    /* compute T(row,col) using the explicit formula */
    seq_t res = S[row][row - col];
    if (col > 0) {
        seq_t sum = 0;
        for (idx_t j = 0; j < col; ++j) {
            seq_t term = fact[j] * S[row][row + 1 + j - col];
            if (j & 1)
                sum -= term;
            else
                sum += term;
        }
        res += (seq_t)(row - col) * sum;
    }
    return res;
}
