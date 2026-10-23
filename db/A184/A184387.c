/*
 * A184387  a(n) = sum of numbers from 1 to sigma(n), where sigma(n) =
 *          A000203(n).
 * https://oeis.org/A184387
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A000203(n)*(A000203(n)+1)/2
 * Range:     u8 n <= 11, u16 n <= 143, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Cache the values of σ(m) for m ≤ current limit */
static seq_t *sigma_cache = NULL;
static idx_t sigma_cache_max = 0;

/* Recompute σ values up to and including limit n (inclusive) */
static void recompute_sigma(idx_t n)
{
    if (n <= sigma_cache_max)
        return;

    /* Allocate a new zero‑initialized array large enough to hold 0…n */
    seq_t *new_cache = (seq_t*)calloc((size_t)(n + 1), sizeof(seq_t));
    if (!new_cache)
        abort(); /* insufficient memory */

    /* Classic divisor sieve: for every i add i to all multiples of i */
    for (idx_t i = 1; i <= n; ++i) {
        seq_t val = (seq_t)i;
        for (idx_t j = i; j <= n; j += i) {
            new_cache[j] += val;
        }
    }

    /* Replace the old cache and free the previous storage */
    free(sigma_cache);
    sigma_cache = new_cache;
    sigma_cache_max = n;
}

/* Public interface: a(n) = σ(n)*(σ(n)+1)/2 */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;            /* below the OEIS offset, not used */
    recompute_sigma(n);
    seq_t sigma = sigma_cache[n];
    return sigma * (sigma + 1) / 2;
}
