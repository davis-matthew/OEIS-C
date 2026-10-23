/*
 * A034891  Number of different products of partitions of n; number of
 *          partitions of n into prime parts (1 included); number of distinct
 *          orders of Abelian subgroups of symmetric group S_n.
 * https://oeis.org/A034891
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = number of partitions of n into 1 and primes
 *            (non‑composite parts); computed by the classic coin‑change DP:
 *            first add part 1, then each prime p
 * Range:     u8 n <= 22, u16 n <= 72, u32 n <= 267, u64 n <= 1103,
 *            u128 n <= 4850, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* Generate all primes ≤ limit.  The output array is malloc‑ed and must be freed by the caller. */
static void gen_primes_upto(idx_t limit, uint64_t **out, size_t *cnt)
{
    if (limit < 2) {
        *out = NULL;
        *cnt = 0;
        return;
    }
    bool *is_prime = (bool *)malloc((limit + 1) * sizeof(bool));
    if (!is_prime) { *out = NULL; *cnt = 0; return; }
    for (idx_t i = 0; i <= limit; ++i) is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;
    for (idx_t p = 2; p * p <= limit; ++p) {
        if (is_prime[p]) {
            for (idx_t j = p * p; j <= limit; j += p)
                is_prime[j] = false;
        }
    }
    size_t num = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (is_prime[i]) ++num;
    uint64_t *pr = (uint64_t *)malloc(num * sizeof(uint64_t));
    size_t k = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (is_prime[i]) pr[k++] = (uint64_t)i;
    free(is_prime);
    *out = pr;
    *cnt = num;
}

static seq_t *dp   = NULL;      /* DP table */
static idx_t  max_n = (idx_t)-1;/* largest n for which dp is valid */
static uint64_t *primes = NULL;  /* primes up to max_n */
static size_t   primes_cnt = 0;

/* Public function required by the harness */
seq_t a(idx_t n)
{
    /* First time: allocate minimal table */
    if (dp == NULL) {
        dp = (seq_t *)malloc(sizeof(seq_t));
        if (!dp) return (seq_t)0;   /* allocation failure → 0 */
        dp[0] = (seq_t)1;
        max_n = 0;
    }

    /* Need to extend the table */
    if (n > max_n) {
        /* Recompute primes up to new n */
        if (primes != NULL) free(primes);
        gen_primes_upto(n, &primes, &primes_cnt);

        /* Allocate/reallocate DP array */
        seq_t *tmp = (seq_t *)realloc(dp, (size_t)(n + 1) * sizeof(seq_t));
        if (!tmp) return (seq_t)0;   /* allocation failure → 0 */
        dp = tmp;

        /* Recompute DP from scratch for 0 … n */
        dp[0] = (seq_t)1;
        for (idx_t i = 1; i <= n; ++i) dp[i] = (seq_t)0;

        /* part 1 */
        for (idx_t i = 1; i <= n; ++i)
            dp[i] += dp[i - 1];

        /* each prime part */
        for (size_t pi = 0; pi < primes_cnt; ++pi) {
            idx_t p = (idx_t)primes[pi];
            for (idx_t i = p; i <= n; ++i)
                dp[i] += dp[i - p];
        }

        max_n = n;
    }

    return dp[n];
}
