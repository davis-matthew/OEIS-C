/*
 * A064574  Number of partitions of n into parts which are all powers of the
 *          same composite.
 * https://oeis.org/A064574
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Number of partitions of n into parts which are all powers
 *            of a single composite integer (excluding the all‑ones partition)
 * Range:     u8 n <= 87, u16 n <= 923, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* static buffer for dynamic programming */
static seq_t *dp = NULL;
static idx_t dp_cap = 0;

/* helper to ensure dp array has capacity for (n+1) entries */
static void ensure_dp_cap(idx_t n)
{
    if (dp_cap < n + 1) {
        seq_t *new_dp = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
        if (dp != NULL) free(dp);
        dp = new_dp;
        dp_cap = n + 1;
    }
}

/* main function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* offset starts at 1, this case is unused */

    /* sieve for primes up to n to identify composites */
    bool *isprime = (bool *)malloc((size_t)(n + 1) * sizeof(bool));
    for (idx_t i = 0; i <= n; ++i) isprime[i] = true;
    if (n >= 0) isprime[0] = false;
    if (n >= 1) isprime[1] = false;
    for (idx_t i = 2; i * i <= n; ++i) {
        if (isprime[i]) {
            for (idx_t j = i * i; j <= n; j += i) isprime[j] = false;
        }
    }

    seq_t total = 0;

    ensure_dp_cap(n);

    /* temporary array for the powers of a composite */
    idx_t powers[32];
    const idx_t MAX_POWERS = 32;  /* enough for n <= 10^12 */

    for (idx_t c = 4; c <= n; ++c) {            /* composite c >= 4 */
        if (isprime[c]) continue;               /* skip primes */

        /* generate powers of c up to n */
        idx_t np = 0;
        idx_t val = 1;
        while (np < MAX_POWERS && val <= n) {
            powers[np++] = val;
            if (val > n / c) break;            /* avoid overflow */
            val *= c;
        }

        /* initialize dp[0] = 1, others 0 */
        dp[0] = (seq_t)1;
        for (idx_t i = 1; i <= n; ++i) dp[i] = (seq_t)0;

        /* unbounded coin-change DP using the powers */
        for (idx_t pi = 0; pi < np; ++pi) {
            idx_t v = powers[pi];
            for (idx_t s = v; s <= n; ++s) {
                dp[s] = dp[s] + dp[s - v];
            }
        }

        /* exclude the all‑ones partition */
        seq_t cnt = dp[n];
        if (cnt > (seq_t)0) cnt = cnt - (seq_t)1;
        total = total + cnt;
    }

    free(isprime);
    return total;
}
