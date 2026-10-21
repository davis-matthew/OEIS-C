/*
 * A090498  Number of divisors of all the numbers from (1/2)n(n-1)+1 to
 *          n(n+1)/2, i.e., tau(1), tau(2)+tau(3), tau(4)+tau(5)+tau(6),
 *          tau(7)+tau(8)+tau(9)+tau(10), ..., where tau(j) is the number of
 *          divisors of j.
 * https://oeis.org/A090498
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k = n(n‑1)/2 + 1}^{n(n+1)/2} τ(k) where τ(k) is the
 *            number of divisors of k
 * Range:     u8 n <= 34, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation pre‑computes τ(k) for all k up to the maximum
   needed high value using a divisor sieve.  Results are summed
   into a seq_t accumulator for each call. */

#include <stdlib.h>
#include <stdint.h>

/* Cache for divisor counts */
static uint32_t *tau_cache = NULL; /* τ(k) for 1 ≤ k ≤ cache_size-1 */
static idx_t cache_size = 0;       /* number of elements cached (inclusive) */

/* Builds τ(k) for all k ≤ N */
static void build_tau(idx_t N)
{
    /* Allocate and zero the array */
    uint32_t *tau = (uint32_t *)calloc((size_t)N + 1U, sizeof(uint32_t));
    if (!tau) return; /* allocation failure; caller will handle */

    /* Sieve to fill τ(k) */
    for (idx_t i = 1; i <= N; ++i)
    {
        for (idx_t j = i; j <= N; j += i)
            ++tau[j];
    }

    /* Replace old cache */
    free(tau_cache);
    tau_cache = tau;
    cache_size = N + 1U; /* cache covers indices 0 … N */
}

seq_t a(idx_t n)
{
    /* Compute the interval for this n */
    idx_t high = n * ((idx_t)n + 1U) / 2U;
    idx_t low  = n * ((idx_t)n - 1U) / 2U + 1U;

    /* Ensure the divisor table is large enough */
    if (high + 1U > cache_size)
        build_tau(high);

    /* Sum τ(k) over the required range */
    seq_t sum = (seq_t)0;
    for (idx_t i = low; i <= high; ++i)
        sum += (seq_t)tau_cache[i];

    return sum;
}
