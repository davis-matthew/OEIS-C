/*
 * A147844  Difference between the number of distinct prime divisors of
 *          (2*n)!/n!^2 and pi(2*n), where pi(x) is the prime counting
 *          function.
 * https://oeis.org/A147844
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = pi(2*n) - D(C(2*n,n)) where pi(x) is the prime‑counting
 *            function and D(m) is the number of distinct prime divisors of m.
 *            The algorithm uses Legendre’s formula to decide whether each
 *            prime up to 2*n divides the central binomial coefficient and
 *            thus counts the distinct divisors. A cached pri...
 * Range:     u8 n <= 91, u16 n <= 91, u32 n <= 91, u64 n <= 91, u128 n <= 91,
 *            big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/build_primes_upto.h"

seq_t a(idx_t n)
{
    static idx_t last_limit = 0;
    static uint64_t *primes = NULL;
    static size_t prime_count = 0;

    /* limit up to which we need primes ----------------------------------- */
    idx_t limit = n + n;        /* 2*n */
    if (limit > last_limit) {
        if (primes) free(primes);
        build_primes_upto(limit, &primes, &prime_count);
        last_limit = limit;
    }

    /* Count distinct prime factors of C(2n,n) ------------------------------ */
    size_t distinct = 0;
    for (size_t i = 0; i < prime_count; ++i) {
        uint64_t p = primes[i];
        if (p > limit) break;

        idx_t exp = 0;
        idx_t pow = p;
        while (pow <= limit) {
            exp += limit / pow;
            exp -= 2 * (n / pow);
            if (pow > limit / p) break;   /* prevent overflow of pow*=p */
            pow *= p;
        }
        if (exp > 0) ++distinct;
    }

    /* pi(2*n) is simply the number of primes ≤ 2*n ----------------------- */
    seq_t pi_val = (seq_t)prime_count;

    /* Final value ---------------------------------------------------------- */
    return pi_val - (seq_t)distinct;
}
