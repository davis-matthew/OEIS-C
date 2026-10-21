/*
 * A060281  Triangle T(n,k) read by rows giving number of labeled mappings (or
 *          functional digraphs) from n points to themselves (endofunctions)
 *          with exactly k cycles, k=1..n.
 * https://oeis.org/A060281
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) is the n‑th term of the triangle T(n,k) read by rows. For
 *            each n find the row r and column c such that n = r*(r-1)/2 + c
 *            (1 ≤ c ≤ r) Then T(r,c) = Σ_{j=0}^{r-1} C(r-1,j) * r^{\,r-1-j} *
 *            s(j+1,c) where C is the binomial coefficient and s(m,p) is the
 *            signless Stirling number of the first kind
 * Range:     u8 n <= 10, u16 n <= 21, u32 n <= 55, u64 n <= 136,
 *            u128 n <= 378, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
/* The utility functions described in the prelude are available
   externally; we only use the integer types `seq_t` and `idx_t`. */

static bool init_done = false;
#define MAX_R 200
static seq_t S[MAX_R+1][MAX_R+1];     /* Stirling numbers of the first kind */
static seq_t C[MAX_R+1][MAX_R+1];     /* Binomial coefficients */
static void init_arrays(void)
{
    if (init_done) return;
    init_done = true;
    /* Stirling numbers */
    S[0][0] = (seq_t)1;
    for (idx_t m = 1; m <= MAX_R; ++m) {
        S[m][0] = (seq_t)0;
        for (idx_t k = 1; k <= m; ++k) {
            seq_t term1 = S[m-1][k-1];
            seq_t term2 = (seq_t)(m-1) * S[m-1][k];
            S[m][k] = term1 + term2;
        }
    }
    /* Binomial coefficients */
    for (idx_t n = 0; n <= MAX_R; ++n) {
        C[n][0] = C[n][n] = (seq_t)1;
        for (idx_t k = 1; k < n; ++k) {
            C[n][k] = C[n-1][k-1] + C[n-1][k];
        }
    }
}

/* Helper: integer square root floor of 64‑bit unsigned value */
static idx_t isqrt_u64(idx_t x)
{
    idx_t r = 0, b = 1ULL << 62;
    while (b) {
        if (x >= r + b) {
            x -= r + b;
            r = (r >> 1) + b;
        }
        b >>= 2;
    }
    return r;
}

/* Main function required by the exercise */
seq_t a(idx_t n)
{
    init_arrays();
    /* find row r (1-indexed) such that r*(r-1)/2 < n ≤ r*(r+1)/2 */
    idx_t r = 0;
    if (n > 0) {
        /* solve r^2 + r - 2n >= 0 */
        idx_t g = isqrt_u64(8ULL * n + 1);
        r = (g - 1) / 2;
        while ((r * (r + 1) / 2) < n) ++r;
        while (r > 0 && ((r - 1) * r / 2) >= n) --r;
    }
    /* column c */
    idx_t prev_total = (r - 1) * r / 2;
    idx_t c = n - prev_total;   /* 1 ≤ c ≤ r */

    /* special case n==1 */
    if (r <= 1) return (seq_t)1;

    /* powers r^e for e = 0..r-1 */
    seq_t power[MAX_R];
    seq_t r_t = (seq_t)r;
    power[0] = (seq_t)1;
    for (idx_t e = 1; e < r; ++e)
        power[e] = power[e-1] * r_t;

    seq_t result = (seq_t)0;
    idx_t n_minus_1 = r - 1;
    for (idx_t j = 0; j <= n_minus_1; ++j) {
        idx_t jj = j + 1;
        if (jj < c) continue;             /* s(j+1,c)==0 when j+1<c */
        seq_t term = C[n_minus_1][j];
        term *= power[n_minus_1 - j];
        term *= S[jj][c];
        result += term;
    }
    return result;
}
