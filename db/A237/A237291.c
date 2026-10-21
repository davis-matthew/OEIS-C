/*
 * A237291  Number of ways to write 2*n - 1 = p + q + r (p <= q <= r) with p,
 *          q, r, pi(p), pi(q), pi(r) all prime, where pi(x) denotes the
 *          number of primes not exceeding x (A000720).
 * https://oeis.org/A237291
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4953, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: count triples of primes p <= q <= r with p+q+r = 2*n-1
   such that p, q, r, pi(p), pi(q), pi(r) are all prime. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Sequence types supplied by the build: seq_t, idx_t */
seq_t a(idx_t n)
{
    /* 2*n - 1 is the target sum */
    idx_t target = (idx_t)2 * n - (idx_t)1;
    if (target < 2) return (seq_t)0;   /* no primes */

    /* Allocate boolean array for primes up to target */
    size_t len = (size_t)target + (size_t)1;
    uint8_t *isPrime = (uint8_t*)malloc(len);
    if (!isPrime) return (seq_t)0;
    for (size_t i = 0; i < len; ++i) isPrime[i] = 1;
    isPrime[0] = isPrime[1] = 0;
    for (idx_t i = 2; (idx_t)i * i <= target; ++i)
        if (isPrime[i])
            for (idx_t j = i * i; j <= target; j += i)
                isPrime[j] = 0;

    /* Prefix sum: piCnt[i] = number of primes <= i */
    uint32_t *piCnt = (uint32_t*)malloc(len * sizeof(uint32_t));
    if (!piCnt) { free(isPrime); return (seq_t)0; }
    piCnt[0] = 0;
    for (idx_t i = 1; i <= target; ++i)
        piCnt[i] = piCnt[i - 1] + (isPrime[i] ? 1 : 0);

    /* isPrimePrime[i] true if i is prime and pi(i) is prime */
    uint8_t *isPrimePrime = (uint8_t*)malloc(len);
    if (!isPrimePrime) { free(isPrime); free(piCnt); return (seq_t)0; }
    for (idx_t i = 0; i <= target; ++i)
        isPrimePrime[i] = (isPrime[i] && isPrime[ piCnt[i] ] ? 1 : 0);

    /* Collect indices of primes whose index is also prime */
    idx_t *ppIdx = (idx_t*)malloc(len * sizeof(idx_t));
    if (!ppIdx) { free(isPrime); free(piCnt); free(isPrimePrime); return (seq_t)0; }
    idx_t ppCnt = 0;
    for (idx_t i = 2; i <= target; ++i)
        if (isPrimePrime[i]) ppIdx[ppCnt++] = i;

    /* Count triples p <= q <= r with sum = target */
    seq_t count = 0;
    for (idx_t ii = 0; ii < ppCnt; ++ii)
    {
        idx_t p = ppIdx[ii];
        if (p > target) break;
        for (idx_t jj = ii; jj < ppCnt; ++jj)
        {
            idx_t q = ppIdx[jj];
            idx_t sum_pq = p + q;
            if (sum_pq > target) break;
            idx_t r = target - sum_pq;
            if (r < q) continue;
            if (r <= target && isPrimePrime[r])
                ++count;
        }
    }

    free(isPrime);
    free(piCnt);
    free(isPrimePrime);
    free(ppIdx);
    return count;
}
