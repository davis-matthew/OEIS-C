/*
 * A098036  First occurrence of primes p such that p = (prime(k) +
 *          prime(k+n))/2 for some positive integer k and n=2, 3, ...
 * https://oeis.org/A098036
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = first prime p such that p = (prime(k)+prime(k+n))/2 for
 *            some k ≥ 1 (n > 1)
 * Range:     u8 n <= 39, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   build_primes_upto, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/build_primes_upto.h"
#include "../utils/is_prime.h"

/* Static cache of primes (values, 0‑based, prime_list[0] = 2). */
static uint64_t *prime_list = NULL; /* NULL until first allocation */
static size_t     prime_count = 0;   /* number of primes currently cached */
static idx_t      sieve_limit = 100; /* current upper bound for the sieve */

/* Ensure that prime_list contains at least `idx` primes.  The array is
   rebuilt from scratch when a larger bound is needed.  The sieve is
   expanded by doubling the numeric limit until enough primes have
   been generated. */
static void
ensure_primes_upto_index(idx_t idx)
{
    if (prime_count >= idx)                 /* already have enough primes */
        return;

    /* Grow the sieve until we have at least `idx` primes.  We
       re‑build the sieve each time because the provided routine
       rebuilds the prime list from scratch for a new limit. */
    while (prime_count < idx) {
        uint64_t *tmp = NULL;
        size_t   cnt = 0;
        sieve_limit *= 2;                     /* expand the numeric bound */
        build_primes_upto(sieve_limit, &tmp, &cnt);
        free(prime_list);
        prime_list = tmp;
        prime_count = cnt;
    }
}

/* Compute a(n) as defined.  Returns 0 for n ≤ 1 (below offset). */
seq_t
a(idx_t n)
{
    if (n <= 1)                     /* sequence starts at n = 2 */
        return 0;

    /* Search for the first k such that (prime(k)+prime(k+n))/2 is prime. */
    for (idx_t k = 1; ; ++k) {
        idx_t idx2 = k + n;          /* index of the second prime */
        ensure_primes_upto_index(idx2); /* guarantee we have both primes */

        uint64_t p1 = prime_list[(size_t)(k  - 1)]; /* prime(k)  */
        uint64_t p2 = prime_list[(size_t)(idx2 - 1)]; /* prime(k+n) */
        uint64_t sum = p1 + p2;

        /* sum must be even for the average to be integral */
        if (sum & 1ULL)
            continue;

        uint64_t avg = sum >> 1;     /* integer average of the two primes */

        /* Check primality of the average.  is_prime() expects an integer. */
        if (is_prime((idx_t)avg))
            return (seq_t)avg;       /* convert to seq_t (big integer if needed) */
    }
}
