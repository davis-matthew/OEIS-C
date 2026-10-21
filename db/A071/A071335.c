/*
 * A071335  Number of partitions of n into sum of at most three primes.
 * https://oeis.org/A071335
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = number of unordered partitions of n into at most three
 *            primes. Computation uses dynamic programming over primes
 *            (coin‑change style)
 * Range:     u8 n <= 376, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Memoised state */
static idx_t          computed_max = 0;
static seq_t *        dp          = NULL;        /* flat array of size (max+1)*4   */
static uint32_t *     primes      = NULL;        /* primes up to computed_max       */
static size_t         prime_cnt   = 0;

/* Recompute tables up to limit N (N >= 1) */
static void recompute(idx_t N)
{
    /* ---------- sieve of Eratosthenes ---------- */
    bool *is_prime = (bool*)calloc((size_t)(N + 1), sizeof(bool));
    if (!is_prime) { /* allocation failed, terminate */
        abort();
    }

    if (N >= 2) is_prime[2] = true;
    for (idx_t i = 3; i <= N; i += 2) is_prime[i] = true;

    for (idx_t i = 3; (i * i) <= N; i += 2) {
        if (!is_prime[i]) continue;
        for (idx_t j = i * i; j <= N; j += 2 * i) {
            is_prime[j] = false;
        }
    }

    /* collect primes */
    prime_cnt = 0;
    for (idx_t i = 2; i <= N; ++i) {
        if (is_prime[i]) ++prime_cnt;
    }
    if (prime_cnt > 0) {
        primes = (uint32_t*)malloc(prime_cnt * sizeof(uint32_t));
        if (!primes) abort();
    } else {
        primes = NULL;
    }

    size_t pos = 0;
    for (idx_t i = 2; i <= N; ++i) {
        if (is_prime[i]) {
            primes[pos++] = (uint32_t)i;
        }
    }
    free(is_prime);

    /* ---------- DP table ---------- */
    idx_t size_dp = (N + 1) * 4;
    seq_t *new_dp = (seq_t*)malloc(size_dp * sizeof(seq_t));
    if (!new_dp) abort();
    for (idx_t i = 0; i < size_dp; ++i) new_dp[i] = 0;
    new_dp[0] = 1;                       /* dp[0][0] = 1 */

    for (size_t pi = 0; pi < prime_cnt; ++pi) {
        idx_t p = (idx_t)primes[pi];
        for (idx_t sum = p; sum <= N; ++sum) {
            seq_t *dp_sum = &new_dp[sum * 4];
            seq_t *dp_prev = &new_dp[(sum - p) * 4];
            dp_sum[1] += dp_prev[0];
            dp_sum[2] += dp_prev[1];
            dp_sum[3] += dp_prev[2];
        }
    }

    /* replace old tables */
    if (dp) free(dp);
    dp = new_dp;
    computed_max = N;
}

/* Public function required by the build */
seq_t a(idx_t n)
{
    if (n == 0) {                           /* offset is 1, but for safety */
        return 0;
    }

    if (n > computed_max) {
        recompute(n);
    }

    seq_t *dpn = &dp[n * 4];
    return dpn[1] + dpn[2] + dpn[3];
}
