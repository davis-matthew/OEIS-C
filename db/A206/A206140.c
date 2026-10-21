/*
 * A206140  G.f.: Sum_{n>=0} x^n/Product_{k=1..n} (1 - Lucas(k)*x^k +
 *          (-1)^k*x^(2*k)).
 * https://oeis.org/A206140
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the coefficient of x^n in * A(x) = Σ_{k≥0} x^k /
 *            ∏_{i=1}^k (1 – Lucas(i)·x^i + (–1)^i·x^{2i}) * * The computation
 *            uses an O(n^2) DP that builds the partial * products P_k(x) =
 *            ∏_{i=1}^k 1/(1 – Lucas(i)x^i + (–1)^i x^{2i}) * and accumulates
 *            all shifted contributions to the final sequence....
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n) {
    /* Handle the trivial case */
    if (n == 0) return (seq_t)1;

    /* Convert for use with malloc */
    size_t sz = (size_t)n + 1;

    /* Allocate working arrays */
    seq_t *lucas  = (seq_t *)malloc(sz * sizeof(seq_t));
    seq_t *dp_cur = (seq_t *)malloc(sz * sizeof(seq_t));
    seq_t *dp_next= (seq_t *)malloc(sz * sizeof(seq_t));
    seq_t *a_arr  = (seq_t *)malloc(sz * sizeof(seq_t));

    if (!lucas || !dp_cur || !dp_next || !a_arr) {
        /* Memory allocation failed – return 0 as a safe fallback */
        free(lucas); free(dp_cur); free(dp_next); free(a_arr);
        return (seq_t)0;
    }

    /* Initialize arrays with zero */
    memset(lucas,  0, sz * sizeof(seq_t));
    memset(dp_cur, 0, sz * sizeof(seq_t));
    memset(dp_next,0, sz * sizeof(seq_t));
    memset(a_arr,  0, sz * sizeof(seq_t));

    /* Pre‑compute Lucas numbers (L_1 = 1, L_2 = 3) */
    lucas[1] = (seq_t)1;
    if (n >= 2) lucas[2] = (seq_t)3;
    for (idx_t i = 3; i <= n; ++i)
        lucas[i] = lucas[i-1] + lucas[i-2];

    /* Base partial product P_0 = 1 */
    dp_cur[0] = (seq_t)1;

    /* The zeroth term of the sequence is 1 */
    a_arr[0] = (seq_t)1;

    /* Main DP loop */
    for (idx_t k = 1; k <= n; ++k) {
        const seq_t Lk = lucas[k];

        /* Compute P_k from P_{k-1} */
        for (idx_t t = 0; t <= n; ++t) {
            seq_t val = dp_cur[t];  /* start with dp_{k-1}[t] */

            if (t >= k)
                val += Lk * dp_next[t-k];   /* + Lk * P_k[t-k] */

            if (t >= 2*k) {
                if ((k & 1) == 0)   /* k even: subtract */
                    val -= dp_next[t-2*k];
                else                /* k odd: add */
                    val += dp_next[t-2*k];
            }

            dp_next[t] = val;
        }

        /* Swap dp_cur and dp_next for the next iteration */
        seq_t *tmp = dp_cur;
        dp_cur = dp_next;
        dp_next = tmp;

        /* Accumulate contributions to the sequence: a[n] += P_k[n-k] */
        idx_t limit = n - k;
        for (idx_t m = 0; m <= limit; ++m)
            a_arr[k + m] += dp_cur[m];
    }

    /* Result is the nth term */
    seq_t result = a_arr[n];

    /* Clean up */
    free(lucas);
    free(dp_cur);
    free(dp_next);
    free(a_arr);

    return result;
}
