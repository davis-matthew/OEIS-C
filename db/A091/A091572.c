/*
 * A091572  Poincaré series [or Poincare series] of the preprojective algebra
 *          of an extended Dynkin diagram of type E_7.
 * https://oeis.org/A091572
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 6*(n+1) + 2*(p(n) – p(n-9)) where p(n) = number of
 *            non‑negative integer solutions of i + 4*j + 6*k = n
 * Range:     u8 n <= 37, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* dynamic programming for p(m) up to m = n   */
    idx_t N = n;
    seq_t *dp = (seq_t *)calloc((size_t)(N + 1), sizeof(seq_t));
    if (!dp) return (seq_t)0;          /* unlikely allocation failure */

    dp[0] = (seq_t)1;                  /* only the empty combination */

    const idx_t weights[3] = {1, 4, 6};

    for (int wi = 0; wi < 3; ++wi) {
        idx_t w = weights[wi];
        for (idx_t m = w; m <= N; ++m) {
            dp[m] += dp[m - w];
        }
    }

    seq_t p_n     = dp[N];
    seq_t p_shift = (N >= 9) ? dp[N - 9] : (seq_t)0;

    seq_t a_val = (seq_t)6 * ((seq_t)n + (seq_t)1) + (seq_t)2 * (p_n - p_shift);

    free(dp);
    return a_val;
}
