/*
 * A212085  Square array A(n,k), n>=1, k>=1, read by antidiagonals: A(n,k) is
 *          the number of n-colorings of the complete bipartite graph K_(k,k).
 * https://oeis.org/A212085
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the n‑th term of the antidiagonal reading of the square
 *            array A(n,k) where A(i,k) = Σ_{j=1..k} (i-j)^k * S2(k,j) *
 *            ∏_{t=0}^{j-1}(i-t), S2(k,j) being the Stirling numbers of the
 *            second kind. i and k are obtained from n by locating the
 *            anti‑diagonal and its position in that diagonal. Thus a...
 * Range:     u8 n <= 18, u16 n <= 40, u32 n <= 96, u64 n <= 218,
 *            u128 n <= 572, big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/big_pow.h"

#define MAX_K 200

static bool initialized = false;
static seq_t S2[MAX_K][MAX_K];
static seq_t fact[MAX_K];

static void init_tables(void)
{
    if (initialized) return;
    initialized = true;

    /* Stirling numbers of the second kind */
    for (size_t n = 0; n < MAX_K; ++n) {
        for (size_t k = 0; k < MAX_K; ++k) S2[n][k] = 0;
    }
    S2[0][0] = 1;
    for (size_t n = 1; n < MAX_K; ++n) {
        for (size_t k = 1; k <= n; ++k) {
            seq_t val1 = S2[n-1][k-1];
            seq_t val2 = (seq_t)k * S2[n-1][k];
            S2[n][k] = val1 + val2;
        }
    }

    /* factorials */
    fact[0] = 1;
    for (size_t i = 1; i < MAX_K; ++i) {
        fact[i] = fact[i-1] * (seq_t)i;
    }
}

/* compute a(n) */
seq_t a(idx_t n)
{
    init_tables();

    /* find diagonal d such that (d-1)*d/2 < n <= d*(d+1)/2 */
    idx_t d = 1;
    while (n > d * (d + 1) / 2) ++d;

    /* position inside diagonal (0‑based) */
    idx_t prev_total = (d - 1) * d / 2;
    idx_t pos = n - prev_total - 1;

    /* coordinates (i,k) */
    idx_t i = pos + 1;
    idx_t k = d - pos;

    seq_t sum = 0;
    idx_t maxj = i > 1 ? i - 1 : 0;
    if (maxj > k) maxj = k;

    for (idx_t j = 1; j <= maxj; ++j) {
        /* falling factorial (i)_j = i*(i-1)*...*(i-j+1) */
        seq_t falling = 1;
        for (idx_t t = 0; t < j; ++t) {
            falling *= (seq_t)(i - t);
        }

        seq_t base = (seq_t)(i - j);
        seq_t pow_val = big_pow(base, (idx_t)k);

        seq_t term = pow_val * S2[k][j] * falling;
        sum += term;
    }

    return sum;
}
