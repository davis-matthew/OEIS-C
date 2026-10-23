/*
 * A031914  a(n) = prime(10*n - 6).
 * https://oeis.org/A031914
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime(10*n - 6)
 * Range:     u8 n <= 6, u16 n <= 654, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <math.h>
#include <stdint.h>          /* for uint64_t */
#include "../utils/build_primes_upto.h"

/* Cached list of primes (1‑based index) */
static uint64_t *primes  = NULL;  /* points to array of primes, 0‑indexed */
static size_t   prime_cnt = 0;    /* number of primes stored */
static idx_t    sieve_limit = 0;  /* upper bound that primes covers */

/*
 * Ensure that primes[] contains at least required_index primes.
 * The function enlarges the sieve limit until the bound on the
 * required prime index is satisfied.
 */
static void
ensure_primes_up_to(idx_t required_index)
{
    /* nothing to do if we already have enough */
    if (required_index <= prime_cnt)
        return;

    /* Estimate an upper bound for the required prime using the known bound
       p_n <= n (log n + log log n) for n >= 6. */
    idx_t new_limit;
    if (required_index < 6) {
        new_limit = 15;                    /* few first primes <  50 */
    } else {
        double dn = (double)required_index;
        /* for large n the bound may exceed 2^63, clamp to maximum idx_t */
        double estimate = dn * (log(dn) + log(log(dn))) + 3.0;
        if (estimate > (double)(SIZE_MAX))
            estimate = (double)(SIZE_MAX);
        new_limit = (idx_t)estimate;
    }

    /* Ensure we increase the limit monotonically; otherwise we waste work. */
    if (new_limit <= sieve_limit)
        new_limit = sieve_limit * 2 + 10;

    /* Free previous list if any */
    free(primes);
    sieve_limit = new_limit;

    /* Build primes up to the new limit.  The helper allocates the array
       and stores the number of primes in prime_cnt. */
    build_primes_upto(sieve_limit, &primes, &prime_cnt);
}

/* Sequence function: returns the (10*n-6)th prime, with a(1)=7 */
seq_t
a(idx_t n)
{
    if (n == 0)               /* offset is 1; undefined below */
        return 0;

    idx_t k = n * 10 - 6;     /* desired prime index (1‑based) */
    ensure_primes_up_to(k);   /* guarantee we have at least k primes */

    /* primes array is 0‑indexed: primes[0] = 2 is prime(1) */
    return (seq_t)primes[k - 1];
}
