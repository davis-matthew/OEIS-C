/*
 * A161298  Number of partitions of n into numbers not divisible by 4 where
 *          every part appears at least 7 times.
 * https://oeis.org/A161298
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 75, u16 n <= 184, u32 n <= 523, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  a(n) = number of partitions of n into numbers not divisible by 4
    where each part appears at least 7 times. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* undefined for n < 1 */

    /* Avoid overflow when converting.  If n cannot be represented in size_t,
       return 0 (unlikely for the intended problem sizes). */
    if (n > SIZE_MAX - 1) return 0;
    size_t N = (size_t)n;

    /* Allocate two arrays for the DP.  Use conditional compilation so the code
       works in both C (malloc/calloc) and C++ (new/delete). */
#ifdef __cplusplus
    seq_t *dp  = new seq_t[N + 1];
    seq_t *wrk = new seq_t[N + 1];
#else
    seq_t *dp  = (seq_t *)calloc(N + 1, sizeof(seq_t));
    seq_t *wrk = (seq_t *)calloc(N + 1, sizeof(seq_t));
#endif
    if (!dp || !wrk) {                     /* allocation failure */
#ifdef __cplusplus
        delete [] dp;
        delete [] wrk;
#else
        free(dp);
        free(wrk);
#endif
        return 0;
    }

    dp[0] = 1; /* empty partition */

    /* Process each part p that is not divisible by 4. */
    for (size_t p = 1; p <= N; ++p) {
        if (p % 4 == 0) continue;          /* skip multiples of 4 */

        size_t offset = 7 * p;              /* minimum copies of p */

        /* For each residue class modulo p, accumulate sums efficiently. */
        for (size_t r = 0; r < p; ++r) {
            seq_t sum = 0;
            for (size_t idx = r; idx <= N; idx += p) {
                if (idx >= offset) sum += dp[idx - offset];
                wrk[idx] = dp[idx] + sum;
            }
        }

        /* Prepare for next part: dp <- wrk   (swap pointers) */
        seq_t *tmp = dp;
        dp = wrk;
        wrk = tmp;
    }

    seq_t result = dp[N];

    /* Free the allocated arrays.  Use matching deallocation. */
#ifdef __cplusplus
    delete [] dp;
    delete [] wrk;
#else
    free(dp);
    free(wrk);
#endif

    return result;
}
