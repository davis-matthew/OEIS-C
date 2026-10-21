/*
 * A081652  Greatest common divisor of n and sum of decimal digits of n-th
 *          prime.
 * https://oeis.org/A081652
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = GCD( n , sum_of_decimal_digits( nth_prime(n) ) )
 * Range:     u8 n <= 55, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   digit_sum, ensure_cache, gcd, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence offset: 1 (a(1) is the first term) */

#include <stddef.h>                 /* for size_t */
#include "../utils/ensure_cache.h"   /* ensure_cache(idx_t n, seq_t **, size_t *) */
#include "../utils/get_prime.h"     /* uint32_t get_prime(idx_t k) */
#include "../utils/digit_sum.h"     /* seq_t digit_sum(seq_t) */
#include "../utils/gcd.h"           /* seq_t gcd(seq_t, seq_t) */

/* Compute the n‑th term of A081652.
   For n == 0 (below the offset) the function returns 0.  */
seq_t a(idx_t n)
{
    /* Return zero for indices below the offset. */
    if (n == 0) return 0;

    /* Static cache for previously computed terms.
       The cache is sized to hold at least n terms. */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;

    /* Ensure sufficient cache capacity. */
    ensure_cache(n, &cache, &cache_cap);

    /* Return a cached value if available. */
    if (cache[n - 1] != 0) return cache[n - 1];

    /* Compute the n‑th prime (prime_1 = 2). */
    uint32_t prime32 = get_prime(n);          /* returns uint32_t */
    seq_t prime_val  = (seq_t)prime32;        /* safe cast – prime < 2^32 for the ranges used */

    /* Sum of decimal digits of the prime. */
    seq_t digit_sum_val = digit_sum(prime_val);

    /* GCD of n and the digit sum.  Cast n to seq_t first. */
    seq_t result = gcd((seq_t)n, digit_sum_val);

    /* Store the computed value in the cache. */
    cache[n - 1] = result;

    return result;
}
