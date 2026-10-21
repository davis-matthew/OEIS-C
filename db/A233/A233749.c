/*
 * A233749  Number of (n+1)X(1+1) 0..5 arrays with every 2X2 subblock having
 *          the sum of the absolute values of the edge differences equal to
 *          10.
 * https://oeis.org/A233749
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 3, u32 n <= 7, u64 n <= 17, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: number of (n+1)×2 arrays over {0..5} with every 2×2 subblock having
   the sum of absolute edge differences equal to 10.
   The algorithm builds the 36×36 adjacency matrix for row pairs and computes
   a(n) = 1ᵀ·Mⁿ·1 by fast exponentiation. */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

static seq_t M_adj[36][36];
static bool M_adj_initialized = false;

/* Build the adjacency matrix once */
static void init_adj_matrix(void)
{
    if (M_adj_initialized) return;
    for (int a = 0; a < 6; ++a) {
        for (int b = 0; b < 6; ++b) {
            int i = a * 6 + b;
            for (int c = 0; c < 6; ++c) {
                for (int d = 0; d < 6; ++d) {
                    int j = c * 6 + d;
                    int s = abs(a - b) + abs(c - d) + abs(a - c) + abs(b - d);
                    M_adj[i][j] = (s == 10) ? 1 : 0;
                }
            }
        }
    }
    M_adj_initialized = true;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* defined only for n >= 1, return 0 for safety */

    init_adj_matrix();

    /* Result vector: starting from all ones because any first row is allowed */
    seq_t res[36];
    for (int i = 0; i < 36; ++i) res[i] = (seq_t)1;

    /* Local copy of the matrix that will be squared during exponentiation */
    seq_t Mpow[36][36];
    for (int i = 0; i < 36; ++i)
        for (int j = 0; j < 36; ++j)
            Mpow[i][j] = M_adj[i][j];

    idx_t exp = n;
    while (exp > 0) {
        if (exp & 1) {
            /* res = res * Mpow */
            seq_t temp[36];
            for (int i = 0; i < 36; ++i) temp[i] = (seq_t)0;
            for (int k = 0; k < 36; ++k) {
                if (res[k] == (seq_t)0) continue;
                for (int j = 0; j < 36; ++j) {
                    if (Mpow[k][j] == (seq_t)0) continue;
                    temp[j] += res[k] * Mpow[k][j];
                }
            }
            for (int i = 0; i < 36; ++i) res[i] = temp[i];
        }

        /* Mpow = Mpow * Mpow */
        seq_t newM[36][36];
        for (int i = 0; i < 36; ++i) {
            for (int j = 0; j < 36; ++j) {
                newM[i][j] = (seq_t)0;
                for (int k = 0; k < 36; ++k) {
                    if (Mpow[i][k] == (seq_t)0 || Mpow[k][j] == (seq_t)0) continue;
                    newM[i][j] += Mpow[i][k] * Mpow[k][j];
                }
            }
        }
        for (int i = 0; i < 36; ++i)
            for (int j = 0; j < 36; ++j)
                Mpow[i][j] = newM[i][j];

        exp >>= 1;
    }

    /* Sum all components of res to obtain a(n) = 1ᵀ·Mⁿ·1 */
    seq_t ans = (seq_t)0;
    for (int i = 0; i < 36; ++i) ans += res[i];
    return ans;
}
