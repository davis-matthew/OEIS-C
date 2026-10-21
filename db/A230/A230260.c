/*
 * A230260  Number of decompositions of 2n into unordered sums of two primes
 *          with an even sum of Hamming weights.
 * https://oeis.org/A230260
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   build_primes_upto, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A230260 – count unordered pairs of primes p,q with p+q=2*n
 *  such that popcount(p)+popcount(q) is even
 *  (i.e. popcount(p) and popcount(q) have the same parity).
 */
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include "../utils/is_prime.h"
#include "../utils/build_primes_upto.h"

static idx_t prime_limit = 0;     /* largest number for which we have primes */
static uint64_t *prime_list = NULL; /* array of primes up to prime_limit */
static size_t prime_cnt = 0;     /* number of primes in prime_list */

/* Ensure that prime_list contains all primes up to `limit`. */
static void ensure_primes(idx_t limit)
{
    if (prime_limit >= limit)
        return;
    /* free previously allocated array, if any */
    free(prime_list);
    build_primes_upto(limit, &prime_list, &prime_cnt);
    prime_limit = limit;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;

    const idx_t target = 2 * n;        /* we need primes up to 2*n */
    ensure_primes(target);

    seq_t count = 0;

    /* iterate over primes p <= target/2 (unordered pairs) */
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = prime_list[i];
        if (p > target / 2)
            break;
        uint64_t q = target - p;
        if (!is_prime(q))
            continue;
        /* popcount(p) and popcount(q) must have same parity */
        if ((__builtin_popcountll(p) & 1) == (__builtin_popcountll(q) & 1))
            ++count;
    }
    return count;
}
