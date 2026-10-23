/*
 * A045854  Number of nonnegative solutions of x1^2 + x2^2 + ... + x24^2 = n.
 * https://oeis.org/A045854
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of ordered 24‑tuples of non‑negative integers
 *            (x1,…,x24) such that x1² + x2² + … + x24² = n. This is the
 *            coefficient of q^n in (1 + q + q^4 + q^9 + q^16 + …)^24. The
 *            computation is performed by dynamic programming: we convolve the
 *            24 identical generating functions of a single square...
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 24, u64 n <= 287,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* integer square root (floor) for uint64_t */
static idx_t int_sqrt_u64(idx_t n)
{
    idx_t r = 0;
    /* fast head‑break for very large n */
    while ((r + 1) * (r + 1) <= n) ++r;
    return r;
}

seq_t a(idx_t n)
{
    /* smallest index check is unnecessary for the specification; we handle n >= 0 */
    size_t N = (size_t)n;

    /* allocate arrays for DP */
    seq_t *dp     = (seq_t*)calloc(N + 1, sizeof(seq_t));
    seq_t *dp_next = (seq_t*)calloc(N + 1, sizeof(seq_t));
    if (!dp || !dp_next) {
        /* on allocation failure simply return 0 (should not happen in controlled environment) */
        free(dp);
        free(dp_next);
        return (seq_t)0;
    }

    dp[0] = (seq_t)1; /* initialise with 0 squares forming 0 */

    /* list of square numbers up to N */
    size_t maxsq = (size_t)int_sqrt_u64(n);
    size_t *sqs = (size_t*)malloc((maxsq + 1) * sizeof(size_t));
    if (!sqs) {
        free(dp);
        free(dp_next);
        return (seq_t)0;
    }
    for (size_t i = 0; i <= maxsq; ++i)
        sqs[i] = i * i;

    /* repeat convolution 24 times (one for each variable) */
    for (int t = 0; t < 24; ++t) {
        /* reset dp_next */
        for (size_t i = 0; i <= N; ++i)
            dp_next[i] = (seq_t)0;

        /* convolution: add contribution of each square */
        for (size_t j = 0; j <= N; ++j) {
            seq_t val = dp[j];
            if (val == (seq_t)0) continue;
            for (size_t k = 0; k <= maxsq; ++k) {
                size_t s = sqs[k];
                size_t idx = j + s;
                if (idx > N) break;
                dp_next[idx] += val;
            }
        }

        /* swap dp and dp_next for next iteration */
        seq_t *tmp = dp;
        dp = dp_next;
        dp_next = tmp;
    }

    seq_t result = dp[N];

    /* clean up */
    free(dp);
    free(dp_next);
    free(sqs);

    return result;
}
