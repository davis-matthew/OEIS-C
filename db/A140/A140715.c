/*
 * A140715  Consider all the integers k where the n-th prime <= k <= the
 *          (n+1)th prime. a(n) is the largest integer that divides at least
 *          two of the integers k.
 * https://oeis.org/A140715
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A140715
 * a(n) = largest integer d that divides at least two numbers in the interval
 * [p_n, p_{n+1}] where p_n is the n-th prime. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/build_primes_upto.h"

seq_t a(idx_t n)
{
    if (n < (idx_t)1)
        return (seq_t)0;   /* undefined for n < 1 */

    /* Prime list lazily initialized and grown as needed */
    static bool initialized = false;
    static uint64_t *primes   = NULL;
    static size_t  prime_cnt  = 0;
    static idx_t   sieve_lim  = 200000;   /* initial limit for the sieve */

    if (!initialized) {
        build_primes_upto(sieve_lim, &primes, &prime_cnt);
        initialized = true;
    } else {
        /* Ensure we have at least (n+1)-th prime in the list */
        while (prime_cnt <= (size_t)(n + 1)) {
            free(primes);
            sieve_lim *= 2;
            build_primes_upto(sieve_lim, &primes, &prime_cnt);
        }
    }

    /* p_n and p_{n+1} */
    uint64_t pn  = primes[(size_t)(n - 1)];
    uint64_t pn1 = primes[(size_t)n];

    uint64_t gap = pn1 - pn;   /* maximum possible divisor */

    /* Search largest divisor d <= gap that has two multiples in [pn, pn1] */
    for (uint64_t d = gap; d >= (uint64_t)2; --d) {
        uint64_t first = ((pn + d - 1) / d) * d;   /* first multiple of d >= pn */
        if (first + d <= pn1)                     /* second multiple also <= pn1 */
            return (seq_t)d;
    }

    /* If no divisor > 1 works, the answer is 1 */
    return (seq_t)1;
}
