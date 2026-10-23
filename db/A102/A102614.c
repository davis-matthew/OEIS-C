/*
 * A102614  Denominators of the reduced fractions of the ratios of the number
 *          of primes less than n over the number of composites less than n.
 * https://oeis.org/A102614
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  frac, nonn
 * Formula:   a(n) = (n - π(n)) / gcd(π(n), n - π(n)) where π(n) is the number
 *            of primes ≤ n. The sequence is defined for n ≥ 1; for n = 1 it
 *            returns 1
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Static cache for primes up to the largest n requested so far. */
static uint8_t *is_composite = NULL;  /* 0 = prime, 1 = composite */
static idx_t   cached_limit   = 0;     /* highest n computed so far */

static seq_t gcd_seq(seq_t a, seq_t b)
{
    while (b) {
        seq_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* Return the denominator of the reduced fraction π(n)/(n-π(n)). */
seq_t a(idx_t n)
{
    /* Handle the special initial term. */
    if (n <= 1)
        return (seq_t)1;

    /* Expand sieve if necessary. */
    if (n > cached_limit) {
        /* Free old table if it exists. */
        if (is_composite)
            free(is_composite);

        /* Allocate new table large enough for [0..n]. */
        idx_t new_limit = n;
        is_composite = (uint8_t *)calloc((size_t)(new_limit + 1), sizeof(uint8_t));
        if (!is_composite)
            return (seq_t)0;  /* out of memory: return zero */

        /* Sieve of Eratosthenes. 0 and 1 are not prime. */
        is_composite[0] = 1;
        if (new_limit >= 1)
            is_composite[1] = 1;

        idx_t limit_sqrt = 0;
        for (; (idx_t)(limit_sqrt + 1) * (idx_t)(limit_sqrt + 1) <= new_limit; ++limit_sqrt)
            ;
        for (idx_t i = 2; i <= limit_sqrt; ++i) {
            if (!is_composite[i]) {
                for (idx_t j = i * i; j <= new_limit; j += i)
                    is_composite[j] = 1;
            }
        }

        cached_limit = new_limit;
    }

    /* Count primes π(n). */
    seq_t pi = 0;
    for (idx_t i = 2; i <= n; ++i) {
        if (!is_composite[i])
            ++pi;
    }

    seq_t composites = (seq_t)n - pi;
    if (composites == 0)
        return (seq_t)1;  /* only occurs for n=1, handled above */

    seq_t g = gcd_seq(pi, composites);
    return composites / g;
}
