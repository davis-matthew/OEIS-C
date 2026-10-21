/*
 * A097456  Integer part of the ratio (number of composites <=n) / (number of
 *          primes <=n).
 * https://oeis.org/A097456
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor( (n - pi(n)) / pi(n) ) where pi(n) is the number of
 *            primes ≤ n
 * Range:     u8 n <= 106, u16 n <= 106, u32 n <= 106, u64 n <= 106,
 *            u128 n <= 106, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static uint8_t *isComp = NULL;   /* 0 → prime, 1 → composite or <2 */
static idx_t sieve_limit = 0;    /* highest n for which sieve is built */

static void build_sieve(idx_t limit)
{
    /* Allocate or resize the sieve array */
    isComp = (uint8_t *)realloc(isComp, (size_t)(limit + 1) * sizeof(uint8_t));
    for (idx_t i = 0; i <= limit; ++i) isComp[i] = 0;

    /* Eratosthenes sieve */
    for (idx_t p = 2; p * p <= limit; ++p) {
        if (!isComp[p]) {
            for (idx_t k = p * p; k <= limit; k += p)
                isComp[k] = 1;
        }
    }
    sieve_limit = limit;
}

seq_t a(idx_t n)
{
    if (n < 2) /* undefined for n<2 – return 0 as placeholder */
        return (seq_t)0;

    /* Rebuild sieve if necessary */
    if (n > sieve_limit)
        build_sieve(n);

    /* Count primes up to n */
    idx_t pi = 0;
    for (idx_t i = 2; i <= n; ++i)
        if (!isComp[i]) ++pi;

    /* Compute floor((n - pi)/pi) */
    idx_t composites = n - pi;
    return (seq_t)composites / (seq_t)pi;
}
