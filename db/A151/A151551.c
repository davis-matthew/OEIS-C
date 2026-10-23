/*
 * A151551  G.f.: (1 + 3x) * Product_{n>=1} (1 + x^(2^n-1) + 2*x^(2^n)).
 * https://oeis.org/A151551
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = [x^n] (1 + 3x) * Prod_{k>=1} (1 + x^(2^k-1) + 2*x^(2^k))
 * Range:     u8 n <= 59, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>


/* Multiply the current DP array by the factor (1 + x^e1 + 2*x^e2).
 * Only use contributions that fit within the length n. */
static void
apply_factor(seq_t *dp, idx_t n, idx_t e1, idx_t e2)
{
    for (idx_t sum = n; ; --sum) {
        seq_t old = dp[sum];
        if (old) {
            if (e1 <= n && sum + e1 <= n)
                dp[sum + e1] += old;                 /* weight 1 */
            if (e2 <= n && sum + e2 <= n)
                dp[sum + e2] += old * (seq_t)2;      /* weight 2 */
        }
        if (sum == 0) break;
    }
}

/* Computes the n-th term of sequence A151551. */
seq_t a(idx_t n)
{
    /* Allocate DP array of size n+1. */
    size_t sz = (size_t)n + 1;
    seq_t *dp = (seq_t *)malloc(sizeof(seq_t) * sz);
    if (!dp) return (seq_t)0;                /* out of memory -> zero */

    /* Initialize dp[0] = 1, others = 0. */
    for (size_t i = 0; i < sz; ++i) dp[i] = (seq_t)0;
    dp[0] = (seq_t)1;

    /* Process each factor (1 + x^(2^k-1) + 2*x^(2^k)) for all k
       with 2^k <= n+1 (so 2^k-1 may still be <= n). */
    for (idx_t pow = (idx_t)2; pow <= n + 1 && pow > 0; pow <<= 1) {
        idx_t add = pow - 1;          /* 2^k-1 */
        apply_factor(dp, n, add, pow);
    }

    /* Apply the outer factor (1 + 3x). */
    seq_t result = dp[n];
    if (n > 0) {
        result += dp[n - 1] * (seq_t)3;
    }

    /* Clean up and return. */
    free(dp);
    return result;
}
