/*
 * A085883  Let r and s be such that r + s = n; a(n) = maximum value of tau(r)
 *          + tau(s).
 * https://oeis.org/A085883
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = maximum over 1 ≤ r < n of τ(r) + τ(n-r), where τ(k) is
 *            the number of positive divisors of k. (Offset 2)
 * Range:     u8 n <= 9578, u16 n <= 9578, u32 n <= 9578, u64 n <= 9578,
 *            u128 n <= 9578, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Compute number of divisors τ(k) for all k ≤ n and
   store them in a static cache.  The cache is recomputed
   from scratch whenever a bigger n is requested. */
static seq_t *tau_cache = NULL;
static idx_t tau_cache_size = 0;

static void recompute_tau(idx_t n)
{
    /* Free old cache if any. */
    if (tau_cache != NULL)
        free(tau_cache);

    /* Allocate new cache of size n+1. */
    tau_cache = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (tau_cache == NULL)
        return; /* In practice, the caller will fail gracefully. */

    /* Initialise all entries to 0. */
    for (idx_t i = 0; i <= n; ++i)
        tau_cache[i] = 0;

    /* Classic divisor sieve: for each i, add 1 to all multiples. */
    for (idx_t i = 1; i <= n; ++i) {
        for (idx_t j = i; j <= n; j += i)
            ++tau_cache[j];
    }

    tau_cache_size = n;
}

seq_t a(idx_t n)
{
    /* The sequence is defined only for n ≥ 2; the behaviour below is irrelevant. */
    if (n < 2)
        return 0;

    /* Ensure τ-values up to n are available. */
    if (n > tau_cache_size)
        recompute_tau(n);

    seq_t best = 0;
    for (idx_t r = 1; r < n; ++r) {
        seq_t sum = tau_cache[r] + tau_cache[n - r];
        if (sum > best)
            best = sum;
    }
    return best;
}
