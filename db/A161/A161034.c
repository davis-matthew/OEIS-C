/*
 * A161034  Number of partitions of n into positive Fibonacci numbers where
 *          every part appears at least 10 times.
 * https://oeis.org/A161034
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 98, u16 n <= 271, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula:
 *   a(n) = number of partitions of n into positive Fibonacci numbers
 *   where every distinct part is used at least 10 times.
 *
 * The implementation uses dynamic programming with the generating
 * function for each Fibonacci number F:
 *   1 + x^{10F} + x^{11F} + x^{12F} + …
 * For each residue class modulo F the contributions can be computed
 * with a simple prefix‑sum technique, giving an overall O(n F) algorithm.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Generate the distinct Fibonacci numbers <= n. */
    seq_t fib[32];
    size_t fib_cnt = 0;
    seq_t a_fib = (seq_t)1;   /* first distinct Fibonacci number 1 */
    seq_t b_fib = (seq_t)2;   /* second distinct Fibonacci number 2 */
    while (a_fib <= (seq_t)n) {
        fib[fib_cnt++] = a_fib;
        seq_t c = a_fib + b_fib;
        a_fib = b_fib;
        b_fib = c;
    }

    /* DP arrays: dp[s] is the number of ways to form sum s
     * using the Fibonacci numbers processed so far.          */
    size_t N = (size_t)n;
#ifdef __cplusplus
    seq_t *dp  = new seq_t[N + 1];
    seq_t *nxt = new seq_t[N + 1];
#else
    seq_t *dp  = (seq_t *)malloc((N + 1) * sizeof(seq_t));
    seq_t *nxt = (seq_t *)malloc((N + 1) * sizeof(seq_t));
#endif
    if (!dp || !nxt) {
#ifdef __cplusplus
        delete[] dp;
        delete[] nxt;
#else
        free(dp);
        free(nxt);
#endif
        return (seq_t)0;
    }

    /* Initialise DP arrays. */
    for (size_t i = 0; i <= N; ++i) {
        dp[i]  = (seq_t)0;
        nxt[i] = (seq_t)0;
    }
    dp[0] = (seq_t)1;   /* empty partition */

    /* Process each Fibonacci number. */
    for (size_t fi = 0; fi < fib_cnt; ++fi) {
        seq_t F = fib[fi];

        /* Start with the option of not using this part. */
        for (size_t i = 0; i <= N; ++i)
            nxt[i] = dp[i];

        /* For each residue class modulo F compute contributions
         * using prefix sums over the arithmetic progression.   */
        for (size_t r = 0; r < (size_t)F && r <= N; ++r) {
            /* Count positions in this residue class. */
            size_t pos  = r;
            size_t cnt  = 0;
            while (pos <= N) {
                ++cnt;
                pos += (size_t)F;
            }
            if (cnt == 0)
                continue;

            /* Allocate prefix array. */
#ifdef __cplusplus
            seq_t *pref = new seq_t[cnt];
#else
            seq_t *pref = (seq_t *)malloc(cnt * sizeof(seq_t));
#endif
            if (!pref)
                continue;   /* skip this residue if allocation fails */

            /* Build prefix sums of dp along the progression. */
            pos = r;
            size_t idx = 0;
            while (pos <= N) {
                pref[idx] = (idx == 0) ? dp[pos] : pref[idx - 1] + dp[pos];
                ++idx;
                pos += (size_t)F;
            }

            /* Add contributions for using this part at least 10 times. */
            pos = r;
            idx = 0;
            while (pos <= N) {
                if (idx >= 10)
                    nxt[pos] += pref[idx - 10];
                ++idx;
                pos += (size_t)F;
            }

            /* Free prefix array. */
#ifdef __cplusplus
            delete[] pref;
#else
            free(pref);
#endif
        }

        /* Prepare for next Fibonacci number. */
        seq_t *tmp = dp;
        dp = nxt;
        nxt = tmp;
    }

    seq_t result = dp[N];

    /* Clean up DP arrays. */
#ifdef __cplusplus
    delete[] dp;
    delete[] nxt;
#else
    free(dp);
    free(nxt);
#endif

    return result;
}
