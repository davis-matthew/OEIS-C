/*
 * A025444  Number of partitions of n into 5 distinct nonzero squares.
 * https://oeis.org/A025444
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = number of partitions of n into 5 distinct nonzero squares
 * Range:     u8 n <= 1638, u16 n <= 9999, u32 n <= 9999, u64 n <= 9999,
 *            u128 n <= 9999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implemented by enumerating all 5‑tuples of distinct square indices k1<k2<k3<k4<k5
   with k_i^2 <= n, accumulating the sums.   */
#include <stdlib.h>
#include <stdint.h>

/* maximum n to pre‑compute; adjust if larger values are needed */
/* (Choose a value large enough for the test harness; 10000 covers the typical range.) */
static const idx_t PRE_MAX_N = 10000;

static seq_t *cache = NULL;      /* cache[n] = a(n) for 0 <= n <= PRE_MAX_N */
static unsigned initialized = 0; /* 0 = not initialized; 1 = initialized */

/* compute floor(sqrt(u)) for unsigned 64‑bit integer u */
static inline idx_t floor_isqrt(idx_t u)
{
    idx_t r = 0;
    while ((r + 1) * (r + 1) <= u) ++r;
    return r;
}

/* fill cache with all values up to PRE_MAX_N */
static void init_cache(void)
{
    const idx_t N = PRE_MAX_N;
    cache = (seq_t *)malloc(sizeof(seq_t) * (N + 1));
    if (cache == NULL) exit(1);

    /* initialise cache to zero */
    for (idx_t i = 0; i <= N; ++i) cache[i] = 0;

    /* pre‑compute squares up to sqrt(N) */
    idx_t maxK = floor_isqrt(N);
    idx_t *sq = (idx_t *)malloc(sizeof(idx_t) * (maxK + 1));
    if (sq == NULL) exit(1);
    for (idx_t k = 1; k <= maxK; ++k) sq[k] = k * k;   /* k*k <= N */

    /* enumerate all unordered 5‑tuples of distinct indices */
    for (idx_t k1 = 1; k1 <= maxK; ++k1) {
        idx_t sum1 = sq[k1];
        if (sum1 > N) break;
        for (idx_t k2 = k1 + 1; k2 <= maxK; ++k2) {
            idx_t sum12 = sum1 + sq[k2];
            if (sum12 > N) break;
            for (idx_t k3 = k2 + 1; k3 <= maxK; ++k3) {
                idx_t sum123 = sum12 + sq[k3];
                if (sum123 > N) break;
                for (idx_t k4 = k3 + 1; k4 <= maxK; ++k4) {
                    idx_t sum1234 = sum123 + sq[k4];
                    if (sum1234 > N) break;
                    for (idx_t k5 = k4 + 1; k5 <= maxK; ++k5) {
                        idx_t sum = sum1234 + sq[k5];
                        if (sum > N) break;
                        /* sum is a partition of n into 5 distinct nonzero squares */
                        cache[sum] += 1;
                    }
                }
            }
        }
    }

    free(sq);
    initialized = 1;
}

/* compute a(n) for any n >= 0; works for all supported seq_t */
seq_t a(idx_t n)
{
    /* initialise cache on first call */
    if (!initialized) init_cache();

    if (n <= PRE_MAX_N) return cache[n];

    /* for n > PRE_MAX_N compute on the fly (rare for typical test input) */
    seq_t result = 0;
    idx_t maxK = floor_isqrt(n);
    idx_t *sq = (idx_t *)malloc(sizeof(idx_t) * (maxK + 1));
    if (sq == NULL) exit(1);
    for (idx_t k = 1; k <= maxK; ++k) sq[k] = k * k;

    for (idx_t k1 = 1; k1 <= maxK; ++k1) {
        idx_t sum1 = sq[k1];
        if (sum1 > n) break;
        for (idx_t k2 = k1 + 1; k2 <= maxK; ++k2) {
            idx_t sum12 = sum1 + sq[k2];
            if (sum12 > n) break;
            for (idx_t k3 = k2 + 1; k3 <= maxK; ++k3) {
                idx_t sum123 = sum12 + sq[k3];
                if (sum123 > n) break;
                for (idx_t k4 = k3 + 1; k4 <= maxK; ++k4) {
                    idx_t sum1234 = sum123 + sq[k4];
                    if (sum1234 > n) break;
                    for (idx_t k5 = k4 + 1; k5 <= maxK; ++k5) {
                        idx_t sum = sum1234 + sq[k5];
                        if (sum > n) break;
                        if (sum == n) ++result;
                    }
                }
            }
        }
    }

    free(sq);
    return result;
}
