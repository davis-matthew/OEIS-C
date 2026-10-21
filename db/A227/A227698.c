/*
 * A227698  a(n) is the number of ways to form the integer n as (p^2 + q^2 -
 *          2)/24, where p and q are primes > 3 (excluding swaps of p and q).
 * https://oeis.org/A227698
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   build_primes_upto, floor_sqrt, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * OEIS sequence A227698
 *
 * Formula:  a(n) = #{ unordered prime pairs (p,q), p,q > 3, primes
 *   such that (p² + q² – 2)/24 = n }.
 *
 * The function is implemented using a dynamic sieve of primes up
 * to the square root of the target value 24·n + 2.  It caches the
 * prime list across calls and only extends it when a larger limit
 * is required.
 *
 * Requires the following utility headers (provided by the build):
 *   build_primes_upto.h   – build_primes_upto()
 *   floor_sqrt.h          – floor_sqrt()
 *   is_prime.h            – is_prime()
 */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>

#include "../utils/build_primes_upto.h"
#include "../utils/floor_sqrt.h"
#include "../utils/is_prime.h"

/* Cached prime sieve data */
static uint64_t *prime_list  = NULL;   /* array of primes > 3, ascending */
static size_t   prime_count = 0;       /* number of primes in prime_list   */
static idx_t    prime_max   = 0;       /* maximum prime in prime_list       */

/* Extend prime sieve to cover all primes <= limit */
static void
ensure_primes_upto(idx_t limit)
{
    if (limit <= prime_max)
        return;                       /* already enough */

    uint64_t *new_list = NULL;
    size_t    new_cnt  = 0;

    /* build_primes_upto allocates prime array internally */
    build_primes_upto(limit, &new_list, &new_cnt);

    /* Replace old table */
    free(prime_list);
    prime_list  = new_list;
    prime_count = new_cnt;
    prime_max   = limit;
}

/* Main sequence function */
seq_t
a(idx_t n)
{
    if (n == 0)  /* index 0 is not used; return 0 for safety */
        return (seq_t)0;

    /* target value for the equation: (p² + q² – 2)/24 = n  →  p² + q² = 24·n + 2 */
    seq_t target = ((seq_t)24) * (seq_t)n + 2;

    /* Determine necessary sieve limit: sqrt(target)   (integral part) */
    idx_t sieve_limit = (idx_t)floor_sqrt(target);

    /* Ensure prime list covers required range */
    ensure_primes_upto(sieve_limit);

    seq_t count = 0;

    for (size_t i = 0; i < prime_count; ++i) {
        seq_t p = (seq_t)prime_list[i];

        /* p² must not exceed target – early exit */
        seq_t p2 = p * p;
        if (p2 > target)
            break;

        seq_t rem = target - p2;          /* candidate q² */

        seq_t q = floor_sqrt(rem);
        if (q * q != rem)                 /* rem not a perfect square */
            continue;

        if (q < 5)                        /* require primes > 3 */
            continue;

        /* Test primality of q */
        if (!is_prime((idx_t)q))
            continue;

        /* Count unordered pair only once: p <= q */
        if (p <= (seq_t)q)
            ++count;
    }

    return count;
}
