/*
 * A033792  Product t2(q^d); d | 33, where t2(q) = theta2(q)/(2*q^(1/4)).
 * https://oeis.org/A033792
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of q^n in ∏_{d|33} t(q^d), where t(q) =
 *            Σ_{k≥0} q^{k(k+1)}
 * Range:     u8 n <= 1523, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   compute_product
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation uses convolution of power‑series whose exponents
   are the triangular numbers multiplied by the divisors of 33
   (1, 3, 11, 33). */

#include <stdlib.h>
#include "../utils/compute_product.h"

static seq_t *cache = NULL;   /* array of coefficients up to current cache_len-1 */
static idx_t cache_len = 0;   /* number of terms stored */

/* Extend the cache up to at least n. */
static void ensure_cache(idx_t n)
{
    if (cache && n < cache_len) return;   /* already have it */
    if (cache) free(cache);
    cache = compute_product(n);
    if (cache) cache_len = n + 1;
}

/* Public function required by the test harness. */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache ? cache[n] : 0;
}
