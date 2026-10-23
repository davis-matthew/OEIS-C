/*
 * A023838  Sum of exponents in prime-power factorization of C(5n,n-1).
 * https://oeis.org/A023838
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum of exponents in prime‑power factorization of C(5n,
 *            n-1)
 * Range:     u8 n <= 679, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/build_primes_upto.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* safety for n < 1 */

    idx_t m = 5 * n;                     /* upper argument of the binomial */
    idx_t k = n - 1;                     /* lower argument */
    idx_t l = 4 * n + 1;                 /* the other factor after subtraction */

    /* static cache of primes up to the maximum m we have seen */
    static idx_t prime_limit = 0;
    static uint64_t *primes = NULL;
    static size_t prime_cnt = 0;

    if (prime_limit < m) {                /* need more primes */
        prime_limit = m;
        build_primes_upto(prime_limit, &primes, &prime_cnt);
    }

    seq_t sum = 0;

    /* iterate over all primes p <= m */
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = primes[i];
        if (p > m) break;

        seq_t e = 0;
        idx_t t;

        /* v_p((5n)!) */
        t = m;
        while (t) { t /= p; e += t; }

        /* subtract v_p((n-1)!) */
        t = k;
        while (t) { t /= p; e -= t; }

        /* subtract v_p((4n+1)!) */
        t = l;
        while (t) { t /= p; e -= t; }

        sum += e;                          /* e is guaranteed >= 0 */
    }

    return sum;
}
