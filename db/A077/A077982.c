/*
 * A077982  Min(P in Partitions(n), Sum(k in P, d(k))) where d(k) = number of
 *          divisors of k (A000005).
 * https://oeis.org/A077982
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9779, u16 n <= 9779, u32 n <= 9779, u64 n <= 9779,
 *            u128 n <= 9779, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula:  a(n) = min_{parts} Σ d(k)
 *   where d(k) = number of divisors of k (A000005) and the sum is over all parts k of any
 *   partition of n.  The implementation uses dynamic programming with
 *   small integer arithmetic for speed.  All values are kept in uint64_t
 *   while only the final result is cast to seq_t for return.
 *
 * Uses:
 *   - For each k (1 ≤ k ≤ n) pre‑computed d(k) by a simple divisor sieve.
 *   - DP: dp[0] = 0; for i = 1..n:
 *         dp[i] = min( d(i),  d(k) + dp[i-k]  for 1 ≤ k < i )
 *
 * Complexity:  O(n²) time  (≈ 50 M elementary operations for n = 10 000)
 *              O(n)   memory.
 *   The algorithm is order‑independent; repeated calls only extend the
 *   stored data.
 */

#include <stdlib.h>
#include <stdint.h>

/* The types seq_t and idx_t are defined by the build environment. */
static uint64_t *dp = NULL;          /* DP values a[0…N] */
static uint64_t *divcnt = NULL;      /* divisor counts d[0…N] (divcnt[0] unused) */
static idx_t   computedN = 0;        /* largest index for which arrays are valid */

/* Ensure that arrays are allocated and up to date to cover index n. */
static void ensure(idx_t n)
{
    /* Allocate / grow the storage if needed. */
    if (dp == NULL) {
        dp      = (uint64_t *)malloc((n + 1) * sizeof(uint64_t));
        divcnt  = (uint64_t *)malloc((n + 1) * sizeof(uint64_t));
        if (!dp || !divcnt) return;               /* memory error – abort silently */
        dp[0]   = 0;
        computedN = 0;
    } else if (n > computedN) {
        dp     = (uint64_t *)realloc(dp,     (n + 1) * sizeof(uint64_t));
        divcnt = (uint64_t *)realloc(divcnt, (n + 1) * sizeof(uint64_t));
        if (!dp || !divcnt) return;               /* memory error – abort silently */
    }

    /* If nothing to extend, return. */
    if (n <= computedN) return;

    /* 1. Compute divisor counts d[1…n] using a simple sieve. */
    for (idx_t i = 1; i <= n; ++i) divcnt[i] = 0;
    for (idx_t i = 1; i <= n; ++i)
        for (idx_t j = i; j <= n; j += i)
            ++divcnt[j];

    /* 2. Compute DP values for indices > computedN. */
    for (idx_t i = computedN + 1; i <= n; ++i) {
        uint64_t best = divcnt[i];                  /* use the part i itself */
        for (idx_t k = 1; k < i; ++k) {
            uint64_t cand = divcnt[k] + dp[i - k];
            if (cand < best) best = cand;
        }
        dp[i] = best;
    }

    computedN = n;
}

/* Public entry point required by the build. */
seq_t a(idx_t n)
{
    ensure(n);
    return (seq_t)dp[n];
}
