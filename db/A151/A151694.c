/*
 * A151694  G.f.: Product_{k>=1} (1 + 2*x^(2^k-1) + 2*x^(2^k)).
 * https://oeis.org/A151694
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of x^n in ∏_{k≥1} (1 + 2x^(2^k−1) + 2x^(2^k))
 * Range:     u8 n <= 57, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* It is computed by dynamic programming over the small set of exponents
   2^k−1 and 2^k that do not exceed the argument n. */

/* The function uses a single allocation per sequence array, but
   allocates a temporary buffer only as large as dp.  Both allocations
   are performed with the C++ new operator if compiled as C++,
   otherwise with the C malloc/calloc functions.  The same is used for
   freeing the buffers. */

#include <stdlib.h>
#ifdef __cplusplus
#include <new>
#endif

seq_t a(idx_t n)
{
    /* Special case n == 0 */
    if (n == (idx_t)0)
        return (seq_t)1;

    /* Allocate two buffers of size n+1.  They are zero‑initialised. */
#if __cplusplus
    seq_t *dp   = new seq_t[n + 1]();   /* value‑initialised to 0  */
    seq_t *tmp  = new seq_t[n + 1]();   /* value‑initialised to 0  */
#else
    seq_t *dp   = (seq_t *)calloc((size_t)(n + 1), sizeof(seq_t));
    seq_t *tmp  = (seq_t *)calloc((size_t)(n + 1), sizeof(seq_t));
#endif

    if (!dp || !tmp)   /* in case of allocation failure */
    {
#if __cplusplus
        delete[] dp;
        delete[] tmp;
#else
        free(dp);
        free(tmp);
#endif
        return (seq_t)0;
    }

    dp[0] = (seq_t)1;          /* initial value of the empty product */

    /* Iterate over k = 1,2,… while 2^k−1 ≤ n. */
    for (idx_t k = 1; ; ++k)
    {
        /* 2^k may overflow if k ≥ 64, but then 2^k−1 > n for all 64‑bit n,
           so we break in that situation. */
        idx_t pow2 = ((idx_t)1) << k;
        idx_t e1   = pow2 - (idx_t)1;  /* exponent 2^k−1  */
        if (e1 > n)
            break;
        idx_t e2 = pow2;               /* exponent 2^k      */

        /* copy current dp into tmp */
        for (idx_t i = 0; i <= n; ++i)
            tmp[i] = dp[i];

        /* add contributions of exponent e1 */
        for (idx_t j = e1; j <= n; ++j)
            tmp[j] += (seq_t)2 * dp[j - e1];

        /* add contributions of exponent e2 (if it does not exceed n) */
        if (e2 <= n)
        {
            for (idx_t j = e2; j <= n; ++j)
                tmp[j] += (seq_t)2 * dp[j - e2];
        }

        /* swap pointers dp<->tmp for next iteration */
#if __cplusplus
        seq_t *swap = dp;
        dp = tmp;
        tmp = swap;
#else
        seq_t *swap = dp;
        dp = tmp;
        tmp = swap;
#endif
    }

    seq_t result = dp[n];

    /* free allocated buffers */
#if __cplusplus
    delete[] dp;
    delete[] tmp;
#else
    free(dp);
    free(tmp);
#endif

    return result;
}
