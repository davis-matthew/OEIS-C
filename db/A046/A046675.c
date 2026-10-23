/*
 * A046675  Expansion of Product_{i>0} (1-x^{p_i}), where p_i are the primes.
 * https://oeis.org/A046675
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = coefficient of x^n in ∏_{p prime} (1 – x^p)
 * Range:     i8 n <= 190, i16 n <= 483, i32 n <= 1402, i64 n <= 4694,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the signed count of partitions of n into distinct primes. */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

/* --- Generate all primes ≤ limit ---------------------------------- */
static void primes_upto(idx_t limit, uint64_t **primes_out, size_t *cnt_out)
{
    if (limit < 2) {
        *primes_out = NULL;
        *cnt_out = 0;
        return;
    }

    size_t sz     = (size_t)(limit + 1);
    uint8_t *is_prime = (uint8_t*)calloc(sz, sizeof(uint8_t));
    if (!is_prime) {
        *primes_out = NULL;
        *cnt_out = 0;
        return;
    }

    for (size_t i = 2; i < sz; ++i)
        is_prime[i] = 1;

    for (idx_t i = 2; (idx_t)i * i <= limit; ++i) {
        if (is_prime[i]) {
            for (idx_t j = i * i; j <= limit; j += i)
                is_prime[j] = 0;
        }
    }

    size_t cnt = 0;
    for (size_t i = 2; i < sz; ++i)
        if (is_prime[i]) ++cnt;

    uint64_t *primes = NULL;
    if (cnt > 0) {
        primes = (uint64_t*)malloc(cnt * sizeof(uint64_t));
        if (primes) {
            size_t idx = 0;
            for (size_t i = 2; i < sz; ++i)
                if (is_prime[i]) primes[idx++] = (uint64_t)i;
        }
    }

    free(is_prime);
    *primes_out = primes;
    *cnt_out    = cnt;
}

/* --- A046675 ------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == (idx_t)0)
        return (seq_t)1;          /* a(0) = 1 */

    /* Obtain all primes up to n. */
    uint64_t *primes     = NULL;
    size_t   prime_cnt   = 0;
    primes_upto(n, &primes, &prime_cnt);

    /* DP array: dp[s] = signed number of ways to obtain sum s. */
    size_t dp_size = (size_t)(n + 1);
    seq_t *dp = (seq_t*)calloc(dp_size, sizeof(seq_t));
    if (!dp) {                    /* memory failure fallback */
        free(primes);
        return (seq_t)0;
    }
    dp[0] = (seq_t)1;              /* empty partition has even parity */

    for (size_t pi = 0; pi < prime_cnt; ++pi) {
        idx_t p = (idx_t)primes[pi];
        for (idx_t s = n; s >= p; --s)
            dp[s] -= dp[s - p];
    }

    seq_t result = dp[n];
    free(dp);
    free(primes);
    return result;
}
