/*
 * A055467  Nonprime numbers k for which phi(k) + sigma(k) is an integer
 *          multiple of the cube of the number of divisors of k.
 * https://oeis.org/A055467
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th nonprime integer k such that phi(k)+sigma(k) is
 *            an integer multiple of (d(k))^3, where phi is Euler's totient,
 *            sigma the sum of divisors, and d the number of divisors
 * Range:     u8 n <= 6, u16 n <= 786, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_props
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/factor_props.h"

static seq_t *cache = NULL;      /* stored sequence values */
static size_t cache_len   = 0;   /* number of values cached   */
static size_t cache_cap   = 0;   /* allocated capacity        */
static idx_t  next_k      = 1;   /* next integer to test      */

/* append a value to the cache, growing if necessary */
static void ensure_cache(void) {
    if (cache_len + 1 > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();          /* terminate on allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }
    /* value will be assigned by caller */
}

/* main sequence function requested by the framework */
seq_t a(idx_t n)
{
    /* 1‑based index: return the n‑th term */
    if (n == 0) return 0;          /* undefined but safe */

    while (cache_len < n) {        /* generate terms until we have n */
        uint64_t k = next_k++;

        /* compute properties */
        seq_t phi, sigma, d;
        factor_props(k, &phi, &sigma, &d);

        seq_t sum = phi + sigma;
        seq_t cubes = d * d * d;

        if (cubes != 0 && (sum % cubes) == 0) {
            ensure_cache();
            cache[cache_len++] = (seq_t)k;
        }
    }
    return cache[(size_t)(n - 1)];
}
