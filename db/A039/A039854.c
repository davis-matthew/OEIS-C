/*
 * A039854  Number of partitions satisfying cn(1,5) <= 1 and cn(4,5) <= 1.
 * https://oeis.org/A039854
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n with at most one part ≡1(mod5)
 *            and at most one part ≡4(mod5)
 * Range:     u8 n <= 22, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Necessary standard headers */
#include <stdlib.h>

/* The function comments forced by the requirements */
seq_t a(idx_t n) {
    /* sums = n + 1 (possible sum values 0..n) */
    idx_t sums = n + 1;
    /* There are 4 flag states: (used1, used4) with each bit 0/1 */
    idx_t total = sums * 4;

    /* Allocate the DP array. Use placement according to the language. */
#ifdef __cplusplus
    seq_t *dp = new seq_t[total];
#else
    seq_t *dp = (seq_t *)calloc((size_t)total, sizeof(seq_t));
#endif
    if (!dp) {
        /* allocation failure – return 0 as a safe fallback */
        return (seq_t)0;
    }

    /* Ensure all entries are zero (necessary for built-in types in C++). */
    for (idx_t i = 0; i < total; ++i) dp[i] = (seq_t)0;

    /* Base case: there is one way to form sum 0 with no forbidden parts. */
    dp[0] = (seq_t)1;  /* used1=0, used4=0, sum=0 */

    /* Main DP: iterate over part size s = 1..n */
    for (idx_t s = 1; s <= n; ++s) {
        int rem = (int)(s % 5);
        /* For each achievable total sum after adding part s */
        for (idx_t sum = s; sum <= n; ++sum) {
            idx_t prev_sum = sum - s;
            /* Loop over all current flag combinations */
            for (int used1 = 0; used1 < 2; ++used1) {
                for (int used4 = 0; used4 < 2; ++used4) {
                    /* Index of state before adding part s */
                    idx_t prev_idx = ((used1 << 1) | used4) * sums + prev_sum;
                    seq_t val = dp[prev_idx];
                    if (!val) continue;  /* no ways to reach that state */

                    /* Determine new flags after adding part s */
                    int new1 = used1;
                    int new4 = used4;
                    if (rem == 1) {
                        if (used1) continue;          /* already had a 1-mod-5 part */
                        new1 = 1;
                    } else if (rem == 4) {
                        if (used4) continue;          /* already had a 4-mod-5 part */
                        new4 = 1;
                    }

                    /* Index of state after adding part s */
                    idx_t new_idx = ((new1 << 1) | new4) * sums + sum;
                    dp[new_idx] = dp[new_idx] + val;
                }
            }
        }
    }

    /* Sum all DP entries that have total sum n (any flag combination). */
    seq_t result = (seq_t)0;
    for (int used1 = 0; used1 < 2; ++used1) {
        for (int used4 = 0; used4 < 2; ++used4) {
            idx_t idx = ((used1 << 1) | used4) * sums + n;
            result = result + dp[idx];
        }
    }

    /* Free the allocated memory */
#ifdef __cplusplus
    delete[] dp;
#else
    free(dp);
#endif
    return result;
}
