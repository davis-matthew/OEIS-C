/*
 * A158827  The 4th iteration of x*C(x) where C(x) is the Catalan function
 *          (A000108).
 * https://oeis.org/A158827
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{0 = x_0 <= x_1 <= x_2 <= x_3 <= x_4 = n-1}
 *            Product_{k=0..3} (x_k + 1) * binomial(2*x_{k+1} - x_k + 1,
 *            x_{k+1} - x_k) /(2*x_{k+1} - x_k + 1)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    idx_t N = n;                       /* Number of indices 0 .. n-1 */
    size_t sz = (size_t)N;

    /* Allocate two working arrays of length N */
    seq_t *dp_prev = (seq_t *)calloc(sz, sizeof(seq_t));
    seq_t *dp_next = (seq_t *)calloc(sz, sizeof(seq_t));
    if (!dp_prev || !dp_next) {
        free(dp_prev);
        free(dp_next);
        return 0;
    }

    /* Base: only x_0 = 0 is allowed, so dp_prev[0] = 1, others 0 */
    dp_prev[0] = 1;

    /* Repeat the 4 transitions */
    for (int step = 0; step < 4; ++step) {
        for (idx_t v = 0; v < N; ++v) {
            seq_t sum = 0;
            /* weight for a = v : always 1 */
            if (dp_prev[v] != 0)
                sum += dp_prev[v] * (seq_t)1;

            seq_t w = 1;  /* weight for current a (starting at v) */

            /* iterate a from v down to 1 and compute weight for a-1 */
            for (idx_t a = v; a > 0; --a) {
                /* a is current lower index, compute weight for a-1 */
                idx_t new_a = a - 1;
                seq_t t = (seq_t)(2 * v - a + 1);              /* 2*v - a + 1 */
                seq_t denom = (seq_t)(v - a + 1) * (seq_t)(a + 1); /* (v-a+1)*(a+1) */

                /* weight update: w = w * a * t / denom */
                w = w * (seq_t)a * t / denom;

                if (dp_prev[new_a] != 0)
                    sum += dp_prev[new_a] * w;
            }

            dp_next[v] = sum;
        }

        /* Swap dp_prev and dp_next for the next step */
        seq_t *tmp   = dp_prev;
        dp_prev      = dp_next;
        dp_next      = tmp;
    }

    seq_t result = dp_prev[N - 1];
    free(dp_prev);
    free(dp_next);
    return result;
}
