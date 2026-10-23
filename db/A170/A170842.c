/*
 * A170842  G.f.: Product_{k>=1} (1 + 2x^(2^k-1) + 3x^(2^k)).
 * https://oeis.org/A170842
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = coefficient of x^n in ∏_{k≥1} (1 + 2·x^{2^k-1} +
 *            3·x^{2^k})
 * Range:     u8 n <= 55, u16 n <= 504, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Handle the trivial case n = 0 */
    if (n == 0) return (seq_t)1;

    /* dp[i] holds the coefficient of x^i after processing some k's */
    const idx_t N = n;
    /* Allocate two working arrays; zero-initialise via calloc */
    seq_t *dp_old = (seq_t *)calloc((size_t)(N + 1), sizeof(seq_t));
    seq_t *dp_new = (seq_t *)malloc((size_t)(N + 1) * sizeof(seq_t));
    if (!dp_old || !dp_new) { /* out of memory – return 0 as a safe fallback */
        free(dp_old); free(dp_new);
        return (seq_t)0;
    }
    dp_old[0] = (seq_t)1;                     /* the constant term 1 */

    /* Iterate over k = 1,2,3,…  while the smallest exponent fits */
    for (idx_t k = 1; ; ++k) {
        idx_t e1 = (idx_t)((idx_t)1 << k) - (idx_t)1;  /* 2^k-1 */
        idx_t e2 = (idx_t)((idx_t)1 << k);            /* 2^k  */

        if (e1 > N && e2 > N) break;                 /* no further contributions */

        /* Copy previous coefficients */
        for (size_t i = 0; i <= N; ++i) dp_new[i] = dp_old[i];

        /* Add contributions from x^{e1} with coefficient 2 */
        if (e1 <= N) {
            seq_t mult = (seq_t)2;
            for (idx_t i = e1; i <= N; ++i) {
                dp_new[i] += dp_old[i - e1] * mult;
            }
        }

        /* Add contributions from x^{e2} with coefficient 3 */
        if (e2 <= N) {
            seq_t mult = (seq_t)3;
            for (idx_t i = e2; i <= N; ++i) {
                dp_new[i] += dp_old[i - e2] * mult;
            }
        }

        /* Swap dp_old and dp_new for next iteration */
        seq_t *tmp = dp_old;
        dp_old = dp_new;
        dp_new = tmp;
    }

    seq_t result = dp_old[N];
    free(dp_old);
    free(dp_new);
    return result;
}
