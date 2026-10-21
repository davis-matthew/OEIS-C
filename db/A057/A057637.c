/*
 * A057637  a(n) is the largest number k such that sigma(k) = n, where sigma
 *          is the sum of divisors function A000203, or 0 if no such k exists.
 * https://oeis.org/A057637
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = largest k such that sigma(k) = n (sigma is the
 *            sum‑of‑divisors function), or 0 if none. For each index n we
 *            sieve divisor sums up to n and then scan backwards for the last
 *            occurrence
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Static cache of sigma values.  */
static seq_t *sigma_cache = NULL;
static idx_t sigma_alloc = 0;

/* Ensure the sigma array contains values for all k ≤ n.  */
static void ensure_sigma(idx_t n)
{
    if (sigma_cache && sigma_alloc >= n)
        return;                          /* already computed up to n */

    /* Grow the cache to at least the requested size, preferably twice as large to amortise reallocs. */
    idx_t new_alloc = n * 2 + 10;
    if (new_alloc < n)                 /* can overflow */
        new_alloc = n + 10;

    seq_t *new_cache = (seq_t *)malloc((new_alloc + 1) * sizeof(seq_t));
    if (!new_cache)                    /* allocation failed */
        return;

    /* initialise with zero */
    for (idx_t i = 1; i <= new_alloc; ++i)
        new_cache[i] = 0;

    /* simple divisor‑sieve: add i to all multiples of i */
    for (idx_t i = 1; i <= new_alloc; ++i) {
        seq_t val_i = (seq_t)i;
        for (idx_t j = i; j <= new_alloc; j += i)
            new_cache[j] += val_i;
    }

    /* replace old cache */
    free(sigma_cache);
    sigma_cache = new_cache;
    sigma_alloc = new_alloc;
}

/* a(n) as described.  */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                      /* out of range – defined as 0 */

    ensure_sigma(n);                  /* make sure sigma_cache has all needed values */

    seq_t target = (seq_t)n;
    seq_t best = 0;

    /* scan descending to find the largest k with sigma(k) == n */
    for (idx_t k = n; k >= 1; --k) {
        if (sigma_cache[k] == target) {
            best = k;
            break;
        }
    }

    return best;
}
