/*
 * A071096  Number of ways to tile hexagon of edges n, n+1, n+2, n, n+1, n+2
 *          with diamonds of side 1.
 * https://oeis.org/A071096
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 6, u128 n <= 9,
 *            big any n
 * Helpers:   seq_mul, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = A008793(n+1)/2
 * A008793(m) = ∏_{i=1..m}∏_{j=1..m}∏_{k=1..m} (i+j+k-1)/(i+j+k-2)
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/seq_mul.h"
#include "../utils/seq_pow.h"

static inline unsigned long long binom2(unsigned long long n) {
    /* C(n,2) = n*(n-1)/2  (works for n >= 2) */
    return (n * (n - 1)) >> 1;
}

/* Count the number of triples 1 ≤ i,j,k ≤ m with i+j+k = s.
   Uses inclusion–exclusion for the upper bound m. */
static unsigned long long count_sum(unsigned long long s, unsigned long long m) {
    if (s < 3 || s > 3 * m) return 0ULL;
    unsigned long long sp = s - 3;          /* s' */
    unsigned long long A  = m - 1;          /* upper bound for each variable minus 1 */
    long long sum = 0;
    for (int t = 0; t <= 3; t++) {
        long long L = (long long)sp - (long long)t * (A + 1);
        if (L < 0) continue;
        unsigned long long term = binom2((unsigned long long)(L + 2));
        if (t == 0) sum += term;
        else if (t == 1) sum -= 3 * term;
        else if (t == 2) sum += 3 * term;
        else if (t == 3) sum -= term;
    }
    return (unsigned long long)sum;
}

seq_t a(idx_t n) {
    /* Special case n = 0 */
    if (n == 0) return (seq_t)1;

    unsigned long long m = (unsigned long long)n + 1ULL;      /* m = n+1 */
    unsigned long long maxSum = 3ULL * m;                    /* maximum sum i+j+k */
    unsigned long long maxV   = 3ULL * m - 1ULL;              /* maximum v = i+j+k-1 */

    /* Allocate array for counts of sums s = 3..maxSum */
    size_t countSize = (size_t)(maxSum + 3);      /* include zeros beyond maxSum */
    unsigned long long *cnt = (unsigned long long *)calloc(countSize, sizeof(unsigned long long));
    if (!cnt) return (seq_t)0;                  /* out of memory */

    for (unsigned long long s = 3ULL; s <= maxSum; s++) {
        cnt[s] = count_sum(s, m);
    }

    /* Sieve for smallest prime factor (spf) up to maxV */
    size_t spfSize = (size_t)(maxV + 1);
    unsigned int *spf = (unsigned int *)calloc(spfSize, sizeof(unsigned int));
    if (!spf) { free(cnt); return (seq_t)0; }
    for (size_t i = 0; i < spfSize; i++) spf[i] = (unsigned int)i;
    for (size_t i = 2; i * i <= maxV; i++) {
        if (spf[i] == (unsigned int)i) {
            for (size_t j = i * i; j <= maxV; j += i) {
                if (spf[j] == (unsigned int)j) spf[j] = (unsigned int)i;
            }
        }
    }

    /* Collect primes and map from prime to index */
    size_t primesCount = 0;
    int *primeIdx = (int *)malloc(spfSize * sizeof(int));
    if (!primeIdx) { free(cnt); free(spf); return (seq_t)0; }
    for (size_t i = 0; i < spfSize; i++) primeIdx[i] = -1;

    unsigned int *primes = (unsigned int *)malloc(spfSize * sizeof(unsigned int));
    if (!primes) { free(cnt); free(spf); free(primeIdx); return (seq_t)0; }

    for (size_t i = 2; i <= maxV; i++) {
        if (spf[i] == (unsigned int)i) {
            primeIdx[i] = (int)primesCount;
            primes[primesCount++] = (unsigned int)i;
        }
    }

    /* Prime exponent accumulation */
    long long *primeExp = (long long *)calloc(primesCount, sizeof(long long));
    if (!primeExp) { free(cnt); free(spf); free(primeIdx); free(primes); return (seq_t)0; }

    for (unsigned long long v = 1; v <= maxV; v++) {
        long long e = (long long)cnt[v + 1] - (long long)cnt[v + 2]; /* e_v */
        if (e == 0) continue;
        unsigned long long t = v;
        while (t > 1U) {
            unsigned int p = spf[t];
            unsigned int cntp = 0;
            while (t % p == 0) { t /= p; cntp++; }
            int pi = primeIdx[p];
            if (pi >= 0) primeExp[pi] += e * (long long)cntp;
        }
    }

    /* Divide by 2 once (a(n) = A008793(m) / 2) */
    if (primeIdx[2] >= 0)
        primeExp[primeIdx[2]] -= 1;

    /* Build the result from prime powers */
    seq_t result = (seq_t)1;
    for (size_t i = 0; i < primesCount; i++) {
        if (primeExp[i] > 0) {
            seq_t pwr = seq_pow((seq_t)primes[i], (idx_t)primeExp[i]);
            result = seq_mul(result, pwr);
        }
    }

    /* Free allocated memory */
    free(cnt);
    free(spf);
    free(primeIdx);
    free(primes);
    free(primeExp);

    return result;
}
