/*
 * A239630  Factored over the Gaussian integers, the least number having n
 *          prime factors, excluding units 1, -1, i, and -i.
 * https://oeis.org/A239630
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 9, u32 n <= 15, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = minimal product of ordinary primes whose
 * weighted sum equals n, with weights:
 *   weight(p) = 1 if p = 2 or p ≡ 3 (mod 4)
 *   weight(p) = 2 if p ≡ 1 (mod 4)              */

/* Standard headers */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

/* --------------------------------------------------------------------- */
/* Compute a(1..∞) by a 0‑1 knapsack over ordinary primes.               */
/* An explicit prime sieve is used so no external helpers are needed.   */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* For n==0 we return 1 (unused but harmless). */
    if (n == 0) return (seq_t)1;

    /* ---------------------------------------------------------------- */
    /* Choose an upper bound for the primes that can be needed.           */
    /* For n up to 10000, primes less than 35*n+200 are sufficient.      */
    /* ---------------------------------------------------------------- */
    idx_t limit = n * 35ULL + 200ULL;
    if (limit < 200ULL) limit = 200ULL;

    /* ---------------------------------------------------------------- */
    /* Simple sieve of Eratosthenes to generate all ordinary primes ≤limit. */
    /* ---------------------------------------------------------------- */
    size_t lim = (size_t)(limit + 1);
    uint8_t *is_comp = (uint8_t *)calloc(lim, sizeof(uint8_t));
    if (!is_comp) exit(EXIT_FAILURE);

    size_t estimated = lim / 10;           /* rough estimate of prime count */
    uint64_t *primes = (uint64_t *)malloc(estimated * sizeof(uint64_t));
    if (!primes) { free(is_comp); exit(EXIT_FAILURE); }
    size_t prime_cnt = 0;

    for (size_t i = 2; i < lim; ++i) {
        if (is_comp[i]) continue;
        /* i is prime */
        if (prime_cnt == estimated) {
            estimated = estimated + estimated / 2 + 1;
            uint64_t *tmp = (uint64_t *)realloc(primes, estimated * sizeof(uint64_t));
            if (!tmp) { free(primes); free(is_comp); exit(EXIT_FAILURE); }
            primes = tmp;
        }
        primes[prime_cnt++] = i;
        if (i * i < lim) {
            for (size_t j = i * i; j < lim; j += i) is_comp[j] = 1;
        }
    }
    free(is_comp);

    /* ---------------------------------------------------------------- */
    /* DP: dp[k] = minimal product achieving weight k  (0 ≤ k ≤ n).    */
    /* A value of 0 denotes an unreachable state.                      */
    /* ---------------------------------------------------------------- */
    seq_t *dp = (seq_t *)calloc((size_t)n + 1, sizeof(seq_t));
    if (!dp) { free(primes); exit(EXIT_FAILURE); }
    dp[0] = (seq_t)1;   /* weight 0 achieved by empty product */

    /* Maximum representable value in seq_t (for overflow checking).   */
    seq_t max_val = (seq_t)~(seq_t)0;
    unsigned __int128 max128 = (unsigned __int128)max_val;

    /* ---------------------------------------------------------------- */
    /* 0‑1 knapsack over the primes.                                    */
    /* ---------------------------------------------------------------- */
    for (size_t p_idx = 0; p_idx < prime_cnt; ++p_idx) {
        uint64_t p = primes[p_idx];
        unsigned weight = 0U;

        if (p == 2ULL)           weight = 1U;
        else if ((p % 4ULL) == 3ULL) weight = 1U;
        else                     weight = 2U;            /* p ≡ 1 (mod 4) */

        if (weight > n) continue;

        seq_t p_seq = (seq_t)p;   /* p fits into seq_t because primes are small */

        for (idx_t cur = n; cur >= weight; --cur) {
            seq_t prev = dp[cur - weight];
            if (prev == (seq_t)0) continue;  /* unreachable */

            /* Compute candidate product in a larger type to avoid overflow. */
            unsigned __int128 cand128 = (unsigned __int128)prev * (unsigned __int128)p_seq;
            if (cand128 > max128) continue;          /* exceeds seq_t range */

            seq_t candidate = (seq_t)cand128;

            if (dp[cur] == (seq_t)0 || candidate < dp[cur])
                dp[cur] = candidate;
        }
    }

    seq_t result = dp[n];
    free(dp);
    free(primes);
    return result;
}
