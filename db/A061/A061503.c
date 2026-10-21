/*
 * A061503  a(n) = Sum_{k=1..n} tau(k^2), where tau is the number of divisors
 *          function A000005.
 * https://oeis.org/A061503
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} tau(k^2) where tau(m) is the number of
 *            positive divisors of m
 * Range:     u8 n <= 32, u16 n <= 1024, u32 n <= 1024, u64 n <= 1024,
 *            u128 n <= 1024, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function uses a simple sieve to obtain the smallest prime factor
   for every integer up to the requested n and then computes
   tau(k^2) via dynamic programming:
   If k = p^e * m with m not divisible by p, then
   tau(k^2) = tau(m^2) * (2*e + 1).
   The result for each argument is exact whenever it fits in seq_t.
   The function is memoised across calls so repeated queries are cheap. */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n) {
    /* Static storage for spf and dp arrays – expanded only once per call
       if a larger n is requested. */
    static idx_t *spf = NULL;
    static size_t spf_cap = 0;
    static seq_t *dp  = NULL;
    static size_t dp_cap = 0;

    if (n == 0) return 0;                        /* offset starts at 1 */

    /* Grow arrays if necessary. */
    if ((size_t)n >= spf_cap) {
        size_t new_cap = n + 1;                  /* include index n */
        spf = (idx_t *)realloc(spf, new_cap * sizeof(idx_t));
        dp  = (seq_t *)realloc(dp,  new_cap * sizeof(seq_t));
        spf_cap = new_cap;
        dp_cap = new_cap;

        /* Initialise spf array. */
        for (size_t i = 0; i <= n; ++i) spf[i] = 0;
        for (size_t i = 2; i <= n; ++i) {
            if (spf[i] == 0) {                  /* i is prime */
                spf[i] = i;
                if (i * i <= n) {
                    for (size_t j = i * i; j <= n; j += i)
                        if (spf[j] == 0) spf[j] = i;
                }
            }
        }

        /* Compute tau(k^2) for each k using dynamic programming. */
        dp[0] = 0;                   /* unused, defined for completeness */
        dp[1] = 1;                   /* tau(1^2) = 1 */
        for (idx_t k = 2; k <= n; ++k) {
            idx_t p = spf[k];
            idx_t m = k;
            idx_t e = 0;
            while (m % p == 0) {
                m /= p;
                ++e;
            }
            dp[k] = dp[m] * ((seq_t)2 * e + 1);
        }
    }

    /* Sum up the values to get a(n). */
    seq_t sum = 0;
    for (idx_t k = 1; k <= n; ++k) sum += dp[k];
    return sum;
}
