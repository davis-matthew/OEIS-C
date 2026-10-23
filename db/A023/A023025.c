/*
 * A023025  Number of partitions of n into 5 unordered relatively prime parts.
 * https://oeis.org/A023025
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} mu(d) * p5(n/d) where p5(m) is the number of
 *            partitions of m into exactly 5 parts (unordered). p5 is computed
 *            via the recurrence p5(n, k) = p5(n-1, k-1) + p5(n-k, k) with
 *            base p5(0,0)=1 and p5(n,0)=0 for n>0
 * Range:     u8 n <= 27, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>

#define MAX_N 20000   /* sufficient for n <= 10000 */

static seq_t part[6][MAX_N + 1];   /* DP table for partitions */
static seq_t p5[MAX_N + 1];        /* p5[n] = part[5][n] */
static signed char mu_arr[MAX_N + 1];  /* Möbius function */
static bool initialized = false;

/* Linear sieve to compute mu[1..MAX_N] */
static void compute_mu(void) {
    unsigned int N = MAX_N;
    bool comp[N + 1];
    memset(comp, 0, sizeof(comp));
    int mu[N + 1];
    uint32_t primes[3000];
    size_t pc = 0;

    mu[1] = 1;
    for (unsigned int i = 2; i <= N; ++i) {
        if (!comp[i]) {
            primes[pc++] = i;
            mu[i] = -1;
        }
        for (size_t j = 0; j < pc; ++j) {
            unsigned int p = primes[j];
            unsigned int ip = i * p;
            if (ip > N) break;
            comp[ip] = true;
            if (i % p == 0) {
                mu[ip] = 0;
                break;
            } else {
                mu[ip] = -mu[i];
            }
        }
    }
    for (unsigned int i = 1; i <= N; ++i)
        mu_arr[i] = (signed char)mu[i];
}

/* Compute partition numbers p(n, k) for all n <= MAX_N, k <= 5 */
static void compute_partitions(void) {
    /* Base cases */
    part[0][0] = 1;
    for (size_t n = 1; n <= MAX_N; ++n) part[0][n] = 0;

    for (size_t k = 1; k <= 5; ++k) {
        for (size_t n = 0; n <= MAX_N; ++n) {
            seq_t val = 0;
            if (n >= 1) val += part[k - 1][n - 1];
            if (n >= k) val += part[k][n - k];
            part[k][n] = val;
        }
    }
    for (size_t n = 0; n <= MAX_N; ++n) p5[n] = part[5][n];
}

/* One-time initialization */
static void init_sequence(void) {
    if (initialized) return;
    initialized = true;
    compute_mu();
    compute_partitions();
}

/* Main function required by the framework */
seq_t a(idx_t n) {
    init_sequence();
    if (n < 5) return (seq_t)0;  /* offset is 5 */

    long long sum = 0;
    idx_t sq = (idx_t)sqrt((double)n);
    for (idx_t i = 1; i <= sq; ++i) {
        if (n % i == 0) {
            /* divisor i */
            signed char mu1 = mu_arr[(size_t)i];
            if (mu1 == 1) sum += (long long)p5[(size_t)(n / i)];
            else if (mu1 == -1) sum -= (long long)p5[(size_t)(n / i)];

            /* paired divisor n/i */
            idx_t d2 = n / i;
            if (d2 != i) {
                signed char mu2 = mu_arr[(size_t)d2];
                if (mu2 == 1) sum += (long long)p5[(size_t)(n / d2)];
                else if (mu2 == -1) sum -= (long long)p5[(size_t)(n / d2)];
            }
        }
    }
    return (seq_t)sum;
}
