/*
 * A237049  Number of ordered ways to write n = i + j + k (0 < i <= j <= k)
 *          with i,j,k not all equal such that sigma(i)*sigma(j)*sigma(k) is a
 *          cube, where sigma(m) denotes the sum of all positive divisors of
 *          m.
 * https://oeis.org/A237049
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_cube_product
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the number of ordered triples (i,j,k) with
   0 < i <= j <= k, i+j+k = n, not all equal,
   such that sigma(i)*sigma(j)*sigma(k) is a perfect cube. */
/* The function uses a cache of divisor sums and recomputes it when a larger n is reached. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_cube_product.h"

/* Cache of sigma values up to the largest n seen so far. */
static uint64_t *sigma_cache = NULL;
static idx_t sigma_cache_n  = 0;

/* Recompute sigma values for numbers 1…new_n. */
static void recompute_sigma(idx_t new_n)
{
    if (sigma_cache != NULL) {
        free(sigma_cache);
    }
    sigma_cache = (uint64_t *)malloc((new_n + 1) * sizeof(uint64_t));
    if (sigma_cache == NULL) {
        /* In a real environment we'd handle allocation failure; here we abort. */
        exit(1);
    }
    for (idx_t m = 0; m <= new_n; ++m) {
        sigma_cache[m] = 0;
    }
    for (idx_t d = 1; d <= new_n; ++d) {
        for (idx_t mul = d; mul <= new_n; mul += d) {
            sigma_cache[mul] += d;
        }
    }
    sigma_cache_n = new_n;
}

/* Ensure the cache contains values up to n. */
static void ensure_sigma(idx_t n)
{
    if (sigma_cache == NULL || n > sigma_cache_n) {
        recompute_sigma(n);
    }
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n < 3) return (seq_t)0;          /* No triple possible. */
    ensure_sigma(n);

    uint64_t count = 0;
    for (idx_t i = 1; i <= n / 3; ++i) {
        idx_t rem_i = n - i;
        idx_t j_max = rem_i / 2;
        for (idx_t j = i; j <= j_max; ++j) {
            idx_t k = rem_i - j;
            if (k < j) continue;
            if (i == j && j == k) continue; /* Skip the all‑equal case. */

            __int128 prod = (__int128)sigma_cache[i] * sigma_cache[j];
            prod *= sigma_cache[k];
            if (is_cube_product(prod))
                ++count;
        }
    }
    return (seq_t)count;
}
