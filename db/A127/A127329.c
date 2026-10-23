/*
 * A127329  Semiprimes equal to the sum of three primes in arithmetic
 *          progression.
 * https://oeis.org/A127329
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 3 * prime(n+2) (conjectured from OEIS)
 * Range:     u8 n <= 21, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/build_primes_upto.h"

/* Static cache of primes */
static uint64_t *primes = NULL;   /* array of primes, primes[0] = 2 */
static size_t   primes_cnt = 0;   /* number of primes stored */
static idx_t    primes_limit = 0; /* upper bound used for current sieve */

/* Ensure that at least (n+2) primes are available (1‑based index). */
static void
ensure_primes(idx_t n)
{
    idx_t target = n + 2;          /* required number of primes (1‑based) */

    if (primes_cnt >= (size_t)target)
        return;                    /* enough primes already cached */

    /* Determine an initial sieve limit.  For the first run use a modest value,
       then double it until we have enough primes. */
    idx_t limit = (primes_limit > 0) ? primes_limit * 2 : 100;

    while (1) {
        if (primes)
            free(primes);          /* discard previous sieve result */

        build_primes_upto(limit, &primes, &primes_cnt);
        primes_limit = limit;

        if (primes_cnt >= (size_t)target)
            break;                 /* requirement satisfied */

        limit *= 2;                /* increase and retry */
    }
}

/* Return the n-th term of the A127329 sequence (n ≥ 1).  The function
   can be called in any order; it uses an internal cache of primes so
   that repeated calls are efficient. */
seq_t
a(idx_t n)
{
    if (n == 0)                   /* offset is 1, so undefined for n=0 */
        return (seq_t)0;

    ensure_primes(n);             /* make sure primes[n+1] exists */

    uint64_t prime = primes[(size_t)(n + 1)];   /* primes are 0‑based   */
    return (seq_t)((seq_t)3 * (seq_t)prime);    /* 3 × that prime      */
}
