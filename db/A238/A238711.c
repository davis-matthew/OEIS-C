/*
 * A238711  Product of all primes p such that 2n - p is also prime.
 * https://oeis.org/A238711
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = product of all primes p such that 2n - p is prime
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 20, u64 n <= 38, u128 n <= 59,
 *            big any n
 * Helpers:   build_primes_upto, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Utilities used for prime generation and primality testing */
#include "../utils/build_primes_upto.h"   /* void build_primes_upto(idx_t limit, uint64_t **out, size_t *cnt); */
#include "../utils/is_prime.h"           /* bool is_prime(idx_t n); */

static uint64_t *prime_list = NULL;    /* list of primes up to current_limit */
static size_t prime_cnt   = 0;          /* number of primes in prime_list */
static idx_t   current_limit = 0;       /* maximum prime value stored in prime_list */

/* Rebuild the prime list up to at least 'limit' */
static void ensure_primes(idx_t limit)
{
    if (limit <= current_limit)
        return;

    /* Free previous list if any */
    free(prime_list);

    uint64_t *tmp = NULL;
    build_primes_upto(limit, &tmp, &prime_cnt);
    prime_list = tmp;
    current_limit = limit;
}

/* Required function for the OEIS test harness */
seq_t a(idx_t n)
{
    /* Valid sequence index starts at 2 */
    if (n < 2)
        return 0;

    /* Ensure we have primes up to 2*n */
    idx_t limit = 2 * n;
    if (limit < 2) /* safety for n = 1 (though not used) */
        limit = 2;
    ensure_primes(limit);

    seq_t prod = 1; /* product accumulator */

    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = prime_list[i];
        if (p > limit)
            break;
        idx_t q = 2 * n - p;
        if (q >= 2 && is_prime(q)) {
            prod *= (seq_t)p;
        }
    }

    return prod;
}
