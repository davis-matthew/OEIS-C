/*
 * A151554  G.f.: (1 + 2x) * Product_{n>=1} (1 + x^(2^n-1) + x^(2^n)).
 * https://oeis.org/A151554
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = b(n) + 2·b(n‑1), where b(n) is the coefficient of x^n in
 *            ∏_{k≥1} (1 + x^{2^k-1} + x^{2^k}), with b(0)=1
 * Range:     u8 n <= 504, u16 n <= 9731, u32 n <= 9731, u64 n <= 9731,
 *            u128 n <= 9731, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Computes A151554: coefficient of x^n in (1+2x)∏_{k≥1}(1+x^{2^k-1}+x^{2^k}) */
seq_t a(idx_t n)
{
    /* Maximum exponent needed from the product part */
    idx_t limit = n;
    /* Allocate DP array of size n+1, zero-initialised */
    size_t sz = (size_t)(limit + 1);
    seq_t *dp = (seq_t *)calloc(sz, sizeof(seq_t));
    if (!dp)
        return (seq_t)0;                /* allocation failure */

    dp[0] = (seq_t)1;                   /* b(0) */

    /* For each k such that 2^k-1 ≤ n, update dp from high to low indices */
    for (idx_t k = 1;; ++k)
    {
        idx_t pow2 = ((idx_t)1 << k);   /* 2^k */
        idx_t exp1 = pow2 - (idx_t)1;   /* 2^k-1 */
        idx_t exp2 = pow2;             /* 2^k */

        if (exp1 > limit)              /* no more contributions */
            break;

        for (idx_t e = limit; ; --e)
        {
            seq_t val = dp[(size_t)e]; /* choose 0 from this k */

            if (e >= exp1)
                val += dp[(size_t)(e - exp1)]; /* choose exponent 2^k-1 */

            if (e >= exp2)
                val += dp[(size_t)(e - exp2)]; /* choose exponent 2^k */

            dp[(size_t)e] = val;

            if (e == 0)
                break;
        }
    }

    /* Combine with the initial factor (1+2x) */
    seq_t result;
    if (n == 0)
        result = dp[0];
    else
    {
        seq_t two = (seq_t)2;
        result = dp[(size_t)n] + two * dp[(size_t)(n - 1)];
    }

    free(dp);
    return result;
}
