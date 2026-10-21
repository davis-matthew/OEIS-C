/*
 * A096436  a(n) = the number of squared primes and 1's needed to sum to n.
 * https://oeis.org/A096436
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = minimal number of 1’s and squared primes summing to n
 *            (computed by dynamic programming with a greedy search over
 *            available squares and 1)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* Cached table of minimal counts */
static seq_t *dp      = NULL;   /* dp[0] is always 0             */
static idx_t  dp_cap  = 0;      /* current capacity (in idx_t)   */

/* Simple integer square root – good enough for the small limits we use. */
static idx_t int_sqrt(idx_t n)
{
    idx_t r = 0;
    while ((r + 1) * (r + 1) <= n) {
        ++r;
    }
    return r;
}

/* Extend dp to support indices up to n (inclusive) */
static void ensure_dp(idx_t n)
{
    if (n < dp_cap) return;            /* already big enough */

    /* Allocate a larger table */
    idx_t new_cap = dp_cap ? dp_cap * 2 : 1;
    if (new_cap <= n) new_cap = n + 1;   /* at least n+1 slots */

    seq_t *new_dp = (seq_t *)realloc(dp, new_cap * sizeof(seq_t));
    if (!new_dp) abort();
    dp = new_dp;
    dp_cap = new_cap;

    /* Re‑initialize the new entries (they may contain garbage) */
    for (idx_t i = (dp_cap/2>0?dp_cap/2:0); i < new_cap; ++i) {
        dp[i] = (seq_t)-1; /* sentinel – will be overwritten below */
    }

    /* Prime sieve up to sqrt(new_cap-1) */
    idx_t limit = int_sqrt(new_cap - 1);
    bool *is_prime = (bool *)calloc(limit + 1, sizeof(bool));
    if (!is_prime) abort();
    for (idx_t i = 2; i <= limit; ++i) is_prime[i] = true;
    for (idx_t p = 2; p * p <= limit; ++p) {
        if (is_prime[p]) {
            for (idx_t q = p * p; q <= limit; q += p) is_prime[q] = false;
        }
    }

    /* Collect squared primes */
    idx_t *squares_idx = (idx_t *)malloc((limit + 1) * sizeof(idx_t));
    if (!squares_idx) abort();
    idx_t sq_cnt = 0;
    for (idx_t p = 2; p <= limit; ++p) {
        if (is_prime[p]) {
            squares_idx[sq_cnt++] = p * p;
        }
    }

    /* Compute dp values for new indices */
    for (idx_t i = 0; i < new_cap; ++i) {
        if (i == 0) {
            dp[i] = 0;          /* base case */
            continue;
        }
        /* Start with using a single 1 */
        seq_t best = dp[i - 1] + 1;
        /* Try every squared prime that fits */
        for (idx_t j = 0; j < sq_cnt && squares_idx[j] <= i; ++j) {
            seq_t cand = dp[i - squares_idx[j]] + 1;
            if (cand < best) best = cand;
        }
        dp[i] = best;
    }

    /* Clean up */
    free(is_prime);
    free(squares_idx);
}

/* Public function required by the test harness */
seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* `n` is never 0 for the OEIS sequence, but safe */
    ensure_dp(n);
    return dp[n];
}
