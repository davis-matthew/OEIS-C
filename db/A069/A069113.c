/*
 * A069113  Squarefree part of C(2n,n), the central binomial numbers: the
 *          smallest number such that a(n)*C(2n,n) is a square.
 * https://oeis.org/A069113
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = the squarefree part of C(2n,n) (smallest integer a such
 *            that a*C(2n,n) is a square)
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 20, u64 n <= 35, u128 n <= 75,
 *            big any n
 * Helpers:   build_primes_upto, mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/build_primes_upto.h"  /* build_primes_upto */
#include "../utils/mul_small.h"          /* mul_small */

seq_t a(idx_t n)
{
    /* static cache of primes up to the largest 2n requested so far */
    static uint64_t *primes = NULL;
    static size_t   prime_cnt = 0;
    static idx_t    primes_limit = 0;

    if (n == 0)                     /* C(0,0)=1, squarefree part is 1 */
        return (seq_t)1;

    idx_t need_limit = n << 1;      /* 2n */

    /* If required limit exceeds cached primes, rebuild cache */
    if (need_limit > primes_limit) {
        free(primes);               /* discard previous array, if any */
        build_primes_upto(need_limit, &primes, &prime_cnt);
        primes_limit = need_limit;
    }

    seq_t result = (seq_t)1;

    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = primes[i];
        if ((idx_t)p > need_limit)
            break;

        /* Legendre exponents: e(p) = e_{2n}(p) - 2*e_n(p) */
        idx_t e = 0;
        idx_t t = need_limit;
        while (t) { t /= p; e += t; }
        t = n;
        while (t) { t /= p; e -= 2 * t; }

        if (e & 1u)                  /* odd exponent -> include p */
            result = mul_small(result, (unsigned int)p);
    }

    return result;
}
