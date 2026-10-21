/*
 * A084993  Total number of parts in all partitions of n into prime parts.
 * https://oeis.org/A084993
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = total number of parts in all partitions of n into prime
 *            parts
 * Range:     u8 n <= 23, u16 n <= 70, u32 n <= 259, u64 n <= 1078,
 *            u128 n <= 4781, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#ifdef __cplusplus
#include <vector>
#endif

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    const size_t N = (size_t)n;

    /* Dynamic programming arrays */
    seq_t *dp;
    seq_t *parts;
#ifdef __cplusplus
    dp = new seq_t[N + 1]();
    parts = new seq_t[N + 1]();
#else
    dp = (seq_t *)malloc((N + 1) * sizeof(seq_t));
    parts = (seq_t *)malloc((N + 1) * sizeof(seq_t));
    if (!dp || !parts) {
        free(dp); free(parts);
        return 0;
    }
    /* zero‑initialize in C (new does it automatically in C++) */
    for (size_t i = 0; i <= N; ++i) { dp[i] = 0; parts[i] = 0; }
#endif

    dp[0] = 1;
    parts[0] = 0;

#ifdef __cplusplus
    /* Sieve of Eratosthenes – vector form */
    std::vector<unsigned char> isprime(N + 1, 1);
    if (N >= 0) isprime[0] = 0;
    if (N >= 1) isprime[1] = 0;
    for (size_t p = 2; p * p <= N; ++p)
        if (isprime[p])
            for (size_t k = p * p; k <= N; k += p)
                isprime[k] = 0;

    for (size_t p = 2; p <= N; ++p) if (isprime[p]) {
        for (size_t j = p; j <= N; ++j) {
            dp[j] += dp[j - p];
            parts[j] += parts[j - p] + dp[j - p];
        }
    }
#else
    /* Sieve of Eratosthenes – raw array */
    unsigned char *isprime = (unsigned char *)malloc((N + 1) * sizeof(unsigned char));
    if (!isprime) {
        free(dp);
        free(parts);
        return 0;
    }
    for (size_t i = 0; i <= N; ++i) isprime[i] = 1;
    if (N >= 0) isprime[0] = 0;
    if (N >= 1) isprime[1] = 0;
    for (size_t p = 2; p * p <= N; ++p)
        if (isprime[p])
            for (size_t k = p * p; k <= N; k += p)
                isprime[k] = 0;

    for (size_t p = 2; p <= N; ++p) {
        if (!isprime[p]) continue;
        for (size_t j = p; j <= N; ++j) {
            dp[j] += dp[j - p];
            parts[j] += parts[j - p] + dp[j - p];
        }
    }
    free(isprime);
#endif

    seq_t result = parts[N];

#ifdef __cplusplus
    delete[] dp;
    delete[] parts;
#else
    free(dp);
    free(parts);
#endif

    return result;
}
