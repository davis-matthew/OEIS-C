/*
 * A226287  a(n) = [n/2]!*[(n+1)/2]!*C([n/2],6)*C([(n+1)/2],6).
 * https://oeis.org/A226287
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(n/2)! * ceil(n/2)! * C(floor(n/2),6) *
 *            C(ceil(n/2),6)
 * Range:     u8 n <= 11, u16 n <= 11, u32 n <= 14, u64 n <= 21, u128 n <= 34,
 *            big any n
 * Helpers:   choose6
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/choose6.h"

static seq_t *fact_cache = NULL;
static idx_t   fact_cache_size = 0;

/* Ensure factorials up to n are precomputed and stored in fact_cache[n]. */
static void ensure_fact_cache(idx_t n)
{
    if (n < fact_cache_size) return;           /* already computed */
    /* Grow cache to hold indices 0 .. n */
    idx_t new_size = n + 1;
    seq_t *new_cache = (seq_t *)realloc(fact_cache, new_size * sizeof(seq_t));
    if (!new_cache) exit(EXIT_FAILURE);
    fact_cache = new_cache;

    if (fact_cache_size == 0) {
        fact_cache[0] = 1;
        fact_cache_size = 1;
    }
    /* Compute missing factorials */
    for (idx_t i = fact_cache_size; i <= n; ++i) {
        fact_cache[i] = fact_cache[i - 1] * (idx_t)i;
    }
    fact_cache_size = new_size;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0; /* undefined for n < 1, but safe */
    /* floor(n/2) and ceil(n/2) */
    idx_t m   = n / 2;          /* floor */
    idx_t m2  = (n + 1) / 2;    /* ceil */

    /* If either part is smaller than 6, the binomial is zero */
    if (m < 6 || m2 < 6) return 0;

    /* Precompute needed factorials */
    ensure_fact_cache(m);
    ensure_fact_cache(m2);

    /* Compute the product */
    seq_t result = fact_cache[m] * fact_cache[m2];
    result = result * choose6(m);
    result = result * choose6(m2);

    return result;
}
