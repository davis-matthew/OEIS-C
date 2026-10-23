/*
 * A121561  The number of iterations of "subtract the largest prime less than
 *          or equal to the current value" to go from n to the limiting value
 *          0 or 1.
 * https://oeis.org/A121561
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 9521, u16 n <= 9521, u32 n <= 9521, u64 n <= 9521,
 *            u128 n <= 9521, big any n
 * Helpers:   build_primes, largest_prime_leq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A121561: The number of iterations of "subtract the largest prime <= current value"
   to go from n to 0 or 1. */

/*  The sequence is computed by repeated subtraction of the largest prime not exceeding the
    current value.  The prime list is generated on demand up to the largest argument seen
    so far.  For n ≥ 2 the result is always ≥ 1; for n ≤ 1 the result is 0. */
#include <stddef.h>
#include <stdlib.h>
#include "../utils/build_primes.h"
#include "../utils/largest_prime_leq.h"

/* static state */
static idx_t *primes = NULL;      /* list of primes up to primes_limit */
static size_t primes_cnt = 0;     /* number of primes in the list */
static idx_t primes_limit = 0;    /* largest n for which the list is built */

/* regenerate prime list up to at least 'limit' */
static void ensure_primes(idx_t limit) {
    if (limit <= primes_limit) return;
    if (primes) free(primes);
    size_t cnt;
    build_primes(limit, &primes, &cnt);
    primes_limit = limit;
    primes_cnt = cnt;
}

seq_t a(idx_t n) {
    if (n < 2) return (seq_t)0; /* a(1)=0, a(0)=0 (though n>=1 here) */

    ensure_primes(n);

    idx_t m = n;
    seq_t steps = 0;
    while (m > 1) {
        idx_t p = largest_prime_leq(m, primes, primes_cnt);
        m = m % p;
        ++steps;
    }
    return steps;
}
