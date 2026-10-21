/*
 * A085887  Let r and s be such that r + s = n; a(n) = minimum value of tau(r)
 *          + tau(s).
 * https://oeis.org/A085887
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = min_{1 <= r <= n-1} ( tau(r) + tau(n-r) ) (OEIS A085887)
 *            where tau(k) is the number of positive divisors of k. The
 *            function computes tau values up to the maximum n requested and
 *            then evaluates the minimum in O(n) per call
 * Range:     u8 n <= 9075, u16 n <= 9075, u32 n <= 9075, u64 n <= 9075,
 *            u128 n <= 9075, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* static cache for divisor counts */
static seq_t *tau_cache = NULL;     /* tau[k] = number of divisors of k */
static idx_t tau_len = 0;           /* current size of tau_cache (size == tau_len) */

/* recompute divisor counts up to n (inclusive) */
static void recompute_tau(idx_t n)
{
    /* allocate large enough array */
    tau_cache = (seq_t *)realloc(tau_cache, (n + 1) * sizeof(seq_t));
    if (tau_cache == NULL)
        exit(EXIT_FAILURE);

    /* initialise to zero and compute tau via simple divisor sieve */
    for (idx_t i = 0; i <= n; ++i)
        tau_cache[i] = 0;

    for (idx_t i = 1; i <= n; ++i)
        for (idx_t j = i; j <= n; j += i)
            ++tau_cache[j];

    tau_len = n + 1;   /* array covers indices 0..n */
}

/* ensure tau counts are available up to n */
static void ensure_tau(idx_t n)
{
    if (n + 1 > tau_len)
        recompute_tau(n);
}

/* ---------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* For n < 2 the sequence is undefined; return 0 for safety. */
    if (n < 2)
        return 0;

    ensure_tau(n);

    /* initial best value from r = 1 */
    seq_t best = tau_cache[1] + tau_cache[n - 1];

    /* iterate r from 2 up to n/2 (inclusive) to cover all unordered partitions */
    for (idx_t r = 2; r <= n / 2; ++r) {
        seq_t sum = tau_cache[r] + tau_cache[n - r];
        if (sum < best)
            best = sum;
    }

    return best;
}
