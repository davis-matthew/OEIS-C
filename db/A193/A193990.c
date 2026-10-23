/*
 * A193990  Number of distinct prime factors <= n of binomial(2*n,n).
 * https://oeis.org/A193990
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #{ p ≤ n : p divides binomial(2*n, n) } – the number of
 *            distinct primes not exceeding n that appear in the central
 *            binomial coefficient. The implementation generates primes up to
 *            the requested n (cached between calls) and tests each with
 *            Legendre's formula
 * Range:     u8 n <= 4518, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/build_primes_upto.h"

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;                /* a(0) is undefined in OEIS */

    /* Cache prime list between calls – enlarge only when required */
    static uint64_t *primes = NULL;
    static size_t prime_cnt = 0;
    static idx_t max_n = 0;

    if (n > max_n) {
        /* Rebuild prime table for larger n */
        free(primes);                 /* free previous allocation if any */
        build_primes_upto(n, &primes, &prime_cnt);
        max_n = n;
    }

    idx_t count = 0;

    for (size_t i = 0; i < prime_cnt && primes[i] <= n; ++i) {
        uint64_t p = primes[i];
        idx_t pp = (idx_t)p;

        /* Exponent of p in (2*n)! */
        idx_t e2n = 0;
        idx_t temp = 2 * n;
        while (temp) {
            temp /= pp;
            e2n += temp;
        }

        /* Exponent of p in n! */
        idx_t en = 0;
        temp = n;
        while (temp) {
            temp /= pp;
            en += temp;
        }

        /* exponent in C(2n,n) = e2n - 2*en; count if positive */
        if (e2n > 2 * en)
            ++count;
    }

    return (seq_t)count;
}
