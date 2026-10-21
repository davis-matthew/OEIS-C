/*
 * A239508  Number of partitions of n into nonprime squarefree numbers, cf.
 *          A000469.
 * https://oeis.org/A239508
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into nonprime squarefree
 *            numbers
 * Range:     u8 n <= 53, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)1;
    }

    /* Allocate helper arrays */
    bool *is_prime = (bool *)calloc((size_t)n + 1, sizeof(bool));
    bool *is_sqf   = (bool *)calloc((size_t)n + 1, sizeof(bool));
    if (!is_prime || !is_sqf) {
        free(is_prime);
        free(is_sqf);
        return (seq_t)0;
    }

    /* Prime sieve */
    for (idx_t i = 2; i <= n; ++i) is_prime[i] = true;
    for (idx_t p = 2; p * p <= n; ++p) {
        if (is_prime[p]) {
            for (idx_t j = p * p; j <= n; j += p) {
                is_prime[j] = false;
            }
        }
    }
    is_prime[0] = is_prime[1] = false;

    /* Squarefree sieve */
    for (idx_t i = 1; i <= n; ++i) is_sqf[i] = true;
    for (idx_t p = 2; p * p <= n; ++p) {
        if (is_prime[p]) {
            idx_t pp = p * p;
            for (idx_t j = pp; j <= n; j += pp) {
                is_sqf[j] = false;
            }
        }
    }

    /* Build list of allowed parts */
    idx_t *parts = (idx_t *)malloc(((size_t)n + 1) * sizeof(idx_t));
    if (!parts) {
        free(is_prime);
        free(is_sqf);
        return (seq_t)0;
    }
    size_t parts_cnt = 0;
    parts[parts_cnt++] = 1;          /* 1 is always allowed */

    for (idx_t m = 2; m <= n; ++m) {
        if (is_sqf[m] && !is_prime[m]) {
            parts[parts_cnt++] = m;
        }
    }

    /* DP for partitions */
    seq_t *dp = (seq_t *)malloc(((size_t)n + 1) * sizeof(seq_t));
    if (!dp) {
        free(is_prime);
        free(is_sqf);
        free(parts);
        return (seq_t)0;
    }
    for (idx_t i = 0; i <= n; ++i) dp[i] = (seq_t)0;
    dp[0] = (seq_t)1;

    for (size_t pi = 0; pi < parts_cnt; ++pi) {
        idx_t p = parts[pi];
        for (idx_t s = p; s <= n; ++s) {
            dp[s] = dp[s] + dp[s - p];
        }
    }

    seq_t result = dp[n];

    /* Free memory */
    free(is_prime);
    free(is_sqf);
    free(parts);
    free(dp);

    return result;
}
