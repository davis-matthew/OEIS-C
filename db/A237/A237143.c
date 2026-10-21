/*
 * A237143  Number of (n+1)X(1+1) 0..3 arrays with the maximum plus the lower
 *          median of every 2X2 subblock equal.
 * https://oeis.org/A237143
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 4, u32 n <= 10, u64 n <= 22, u128 n <= 46,
 *            big any n
 * Helpers:   lower_median_plus_max, mat_copy, mat_identity, mat_zero
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) counts the number of (n+1)×2 arrays with entries 0..3
 *           such that for every 2×2 subblock the sum of its maximum
 *           and its lower median is the same constant (independent of
 *           the subblock).  The sequence is computed by treating each
 *           row as a state of 4×4 = 16 possibilities and exponentiating
 *           the corresponding 16×16 adjacency matrices.
 *
 * Implementation: For each possible sum S = 0..6 we pre‑compute a 16×16
 *                 adjacency matrix A[S] where A[S][i][j] = 1 iff the
 *                 2×2 block formed by rows i and j has that sum.
 *                 Then a(n) = Σ_{S=0..6} Σ_{i,j} (A[S]^n)_{i,j}.
 *                 The matrix powers are obtained by fast exponentiation
 *                 (O(log n) multiplications) using standard
 *                 seq_t arithmetic.
 */

#include <stdbool.h>
#include "../utils/lower_median_plus_max.h"
#include "../utils/mat_copy.h"
#include "../utils/mat_identity.h"
#include "../utils/mat_zero.h"

/* -----------------  Auxiliary matrix routines  ----------------- */

/* Standard 16×16 matrix multiplication:  C = A × B   */
/* Assumes C is zeroed before the call.  */
static void mat_mul(const seq_t A[16][16], const seq_t B[16][16], seq_t C[16][16]) {
    seq_t tmp;
    for (idx_t i = 0; i < 16; ++i) {
        for (idx_t j = 0; j < 16; ++j) {
            C[i][j] = 0;
            for (idx_t k = 0; k < 16; ++k) {
                tmp = A[i][k] * B[k][j];
                C[i][j] += tmp;
            }
        }
    }
}

/* -----------------  Lower median + maximum sum  ----------------- */

/* -----------------  Pre‑computed adjacency matrices  ----------------- */

static bool init_adj_matrices = false;
static seq_t adj[7][16][16];   /* adj[s][i][j] = 1 if sum == s */

/* Build the adjacency matrices once.  */
static void init_adj(void) {
    if (init_adj_matrices) return;

    /* Zero all entries */
    for (idx_t s = 0; s <= 6; ++s)
        for (idx_t i = 0; i < 16; ++i)
            for (idx_t j = 0; j < 16; ++j)
                adj[s][i][j] = 0;

    /* For each pair of states (i,j) compute the sum and set edge */
    for (idx_t i = 0; i < 16; ++i) {
        int a = i / 4;
        int b = i % 4;
        for (idx_t j = 0; j < 16; ++j) {
            int c = j / 4;
            int d = j % 4;
            int s = lower_median_plus_max(a, b, c, d);
            if (s >= 0 && s <= 6)
                adj[s][i][j] = 1;
        }
    }
    init_adj_matrices = true;
}

/* -----------------  Sequence function  ----------------- */

seq_t a(idx_t n) {
    /* n >= 1 by specification; for safety handle n==0 -> 0 */
    if (n == 0) return 0;

    init_adj();

    seq_t answer = 0;
    idx_t exp = n;

    /* Temporary matrices used during exponentiation */
    seq_t base[16][16];
    seq_t result[16][16];
    seq_t tmp[16][16];

    for (idx_t s = 0; s <= 6; ++s) {
        /* Copy adjacency matrix for this sum into base */
        mat_copy(adj[s], base);

        /* result = identity */
        mat_identity(result);

        idx_t e = exp;
        while (e > 0) {
            if (e & 1) {
                mat_mul(result, base, tmp);  /* result = result * base */
                mat_copy(tmp, result);
            }
            mat_mul(base, base, tmp);          /* base = base * base */
            mat_copy(tmp, base);
            e >>= 1;
        }

        /* Sum all entries of result into answer */
        for (idx_t i = 0; i < 16; ++i)
            for (idx_t j = 0; j < 16; ++j)
                answer += result[i][j];
    }

    return answer;
}
