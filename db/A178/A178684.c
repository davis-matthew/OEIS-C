/*
 * A178684  Partial sums of cardinalities of coalition sets A095941.
 * https://oeis.org/A178684
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{k=1..n} [ 2^k – 1 – Σ_{j=1..k} M(j) ] where M(j) is
 *            the number of subsets of {1,…,j‑1} whose sum is < j. The
 *            function builds the needed values incrementally, using a DP
 *            array that keeps the number of ways to obtain each possible sum
 *            with the current set of numbers
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   alloc_seq_array, mul_small, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/mul_small.h"     /* mul_small(seq_t, unsigned int) */
#include "../utils/pow2.h"          /* pow2(idx_t)        */
#include "../utils/alloc_seq_array.h" /* alloc_seq_array(size_t) */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* DP array for subset–sum counts.  Size n suffices because for
       each k we only need sums up to k‑1 (≤ n‑1). */
    idx_t dp_size = n;                 /* indices 0 … n‑1 */
    seq_t *dp = alloc_seq_array(dp_size);
    /* zero‑initialize the array */
    for (idx_t i = 0; i < dp_size; ++i)
        dp[i] = 0;
    dp[0] = 1;                         /* empty set */

    seq_t total_violating = 0;   /* Σ_{j=1..k} M(j)  */
    seq_t a_sum          = 0;   /* running sum a(k)   */
    seq_t power_of_two   = 1;   /* 2^0 */

    for (idx_t k = 1; k <= n; ++k) {
        /* update 2^k */
        power_of_two = mul_small(power_of_two, 2);   /* now = 2^k */

        /* compute M(k) = Σ_{s=0}^{k-1} dp[s]  (subsets of {1..k-1} with sum < k) */
        seq_t Mk = 0;
        for (idx_t s = 0; s < k; ++s)
            Mk += dp[s];

        total_violating += Mk;

        /* s(k) = (2^k – 1) – total_violating */
        seq_t sk = power_of_two;
        sk -= 1;
        sk -= total_violating;

        a_sum += sk;

        /* add element k to DP array: update sums in reverse order */
        if (k < dp_size) {  /* only if k <= n‑1, update needed for future k */
            for (idx_t s = dp_size - 1; s >= k; --s)
                dp[s] += dp[s - k];
        }
    }

    return a_sum;
}
