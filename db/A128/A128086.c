/*
 * A128086  A diagonal of the triangle A128084 of coefficients of q in the
 *          q-analog of the even double factorials: a(n) = A128084(n,n).
 * https://oeis.org/A128086
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of q^n in ∏_{k=1}^{n} * (1 + q + q² + … +
 *            q^{2k‑1})
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 18, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * This computation uses a DP that keeps the vector of
 * coefficients of the current partial product.  For each
 * new factor (1+q+…+q^{2k-1}) we perform a sliding‑window
 * sum over the previous coefficient vector.  The algorithm
 * is O(n²) and works for all n ≥ 0.  The array is allocated
 * with calloc and cast to (seq_t*) so that the code compiles
 * under both C11 and C++17 even when seq_t is an arbitrary
 * precision integer class.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* a(0) = 1 */
    if (n == 0)
        return (seq_t)1;

    /* Allocate coefficient vectors of size n+1 */
    size_t N = (size_t)n;
    seq_t *dp = (seq_t *)calloc(N + 1, sizeof(seq_t));
    seq_t *newdp = (seq_t *)calloc(N + 1, sizeof(seq_t));
    if (!dp || !newdp) {
        /* Allocation failed – return 0 as sentinel */
        free(dp);
        free(newdp);
        return (seq_t)0;
    }

    /* Initial polynomial (empty product) */
    dp[0] = (seq_t)1;

    /* Iterate over factors k = 1 .. n */
    for (idx_t k = 1; k <= n; ++k) {
        idx_t limit = 2 * k - 1;           /* highest exponent inside factor k */
        seq_t window = (seq_t)0;

        /* Sliding‑window sum to multiply dp by (1+q+…+q^{limit}) */
        for (idx_t s = 0; s <= n; ++s) {
            window += dp[s];
            if (s > limit)
                window -= dp[s - limit - 1];
            newdp[s] = window;
        }

        /* Prepare for next iteration: swap dp and newdp */
        seq_t *tmp = dp;
        dp = newdp;
        newdp = tmp;
    }

    seq_t result = dp[n];

    free(dp);
    free(newdp);
    return result;
}
