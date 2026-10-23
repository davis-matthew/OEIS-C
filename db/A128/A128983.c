/*
 * A128983  Rightmost position of n in A089625, 0 if absent.
 * https://oeis.org/A128983
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = maximum binary number obtained from any subset of primes
 *            that sum to n, where the j‑th bit (0‑based) corresponds to the
 *            j‑th prime (in ascending order). This value equals the rightmost
 *            (largest) index of n in A089625
 * Range:     u8 n <= 22, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* NOTE: The implementation is fully compatible with both C11 and C++17. It does not use
   any C++ only features and relies only on standard C headers. */

#pragma once
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* sequence is undefined below offset 1 */

    /* ---------- Sieve primes up to n ---------- */
    int limit = (int)n;
    bool *isPrime = (bool*)malloc((limit + 1) * sizeof(bool));
    if (!isPrime) return 0;               /* allocation failure protection */

    for (int i = 0; i <= limit; ++i) isPrime[i] = true;
    if (limit >= 0) isPrime[0] = false;
    if (limit >= 1) isPrime[1] = false;

    for (int i = 2; (int)((long long)i * i) <= limit; ++i)
        if (isPrime[i])
            for (int j = i * i; j <= limit; j += i)
                isPrime[j] = false;

    /* Count primes and store them */
    int primesCount = 0;
    for (int i = 2; i <= limit; ++i)
        if (isPrime[i]) ++primesCount;

    uint64_t *primes = (uint64_t*)malloc(primesCount * sizeof(uint64_t));
    if (!primes) { free(isPrime); return 0; }

    int idx = 0;
    for (int i = 2; i <= limit; ++i)
        if (isPrime[i]) primes[idx++] = (uint64_t)i;

    free(isPrime);  /* prime sieve no longer needed */

    /* ---------- Subset-sum DP for existence ---------- */
    bool *reachable = (bool*)malloc((n + 1) * sizeof(bool));
    if (!reachable) {
        free(primes);
        return 0;
    }
    for (int i = 0; i <= n; ++i) reachable[i] = false;
    reachable[0] = true;

    for (int pIdx = 0; pIdx < primesCount; ++pIdx) {
        int p = (int)primes[pIdx];
        for (int s = n; s >= p; --s) {
            if (reachable[s - p]) reachable[s] = true;
        }
    }

    if (!reachable[n]) {                 /* n cannot be represented as sum of distinct primes */
        free(primes);
        free(reachable);
        return 0;
    }

    /* ---------- Greedy selection of maximal primes to build the largest binary mask ---------- */
    seq_t mask = 0;
    int rem = (int)n;

    for (int pIdx = primesCount - 1; pIdx >= 0; --pIdx) {
        int p = (int)primes[pIdx];
        if (rem >= p && reachable[rem - p]) {
            mask |= ((seq_t)1 << pIdx);
            rem -= p;
        }
    }

    free(primes);
    free(reachable);
    return mask;
}
