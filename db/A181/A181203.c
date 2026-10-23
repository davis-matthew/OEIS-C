/*
 * A181203  a(n)=0!*1!*2!*3!*4!*(5*n)!*10*n*(n-1) /
 *          ((n)!*(n+1)!*(n+2)!*(n+3)!*(n+4)!).
 * https://oeis.org/A181203
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 0!*1!*2!*3!*4! * (5*n)! * 10*n*(n-1) / (n! * (n+1)! *
 *            (n+2)! * (n+3)! * (n+4)!)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 7, u128 n <= 13,
 *            big any n
 * Helpers:   build_primes_upto, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(1) = 0 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"   /* build_primes_upto() */
#include "../utils/seq_pow.h"            /* seq_pow(seq_t base, idx_t exp) */

/* Compute a(n) for n >= 1 */
seq_t a(idx_t n)
{
    /* Offset: first term is a(1)=0 */
    if (n <= 1) return 0;

    /* Limit for primes: need primes up to 5*n */
    idx_t limit = 5 * n;

    /* Static cache of primes up to the largest limit seen so far */
    static uint64_t *primes   = NULL;
    static size_t    primes_cnt = 0;
    static idx_t    primes_limit = 0;

    if (limit > primes_limit) {
        if (primes) free(primes);
        build_primes_upto(limit, &primes, &primes_cnt);
        primes_limit = limit;
    }

    /* Base exponents from the constant 2880 = 2^6 * 3^2 * 5^1 */
    const int base_expo_2 = 6;
    const int base_expo_3 = 2;
    const int base_expo_5 = 1;

    seq_t res = 1;

    for (size_t i = 0; i < primes_cnt; ++i) {
        uint64_t p = primes[i];
        idx_t e_num   = 0;          /* exponent of p in (5*n)!          */
        idx_t e_denom = 0;          /* exponent of p in product of (n..n+4)! */

        /* exponent in (5*n)!    */
        idx_t tmp = limit;
        while (tmp) { tmp /= p; e_num += tmp; }

        /* exponent in each (n+k)! for k=0..4 */
        for (idx_t k = 0; k <= 4; ++k) {
            tmp = n + k;
            while (tmp) { tmp /= p; e_denom += tmp; }
        }

        /* exponent contributed by the constant 2880*n*(n-1)   */
        idx_t e_const = 0;
        if (p == 2) e_const += base_expo_2;
        if (p == 3) e_const += base_expo_3;
        if (p == 5) e_const += base_expo_5;

        /* exponents of p in n and (n-1) */
        idx_t exp_n  = 0;
        idx_t tmp_n  = n;
        while (tmp_n % p == 0) { tmp_n /= p; ++exp_n; }

        idx_t exp_n1 = 0;
        idx_t tmp_n1 = n - 1;
        while (tmp_n1 % p == 0) { tmp_n1 /= p; ++exp_n1; }

        e_const += exp_n + exp_n1;

        /* total exponent for prime p in the final result */
        idx_t e_total = (idx_t)(e_num - e_denom + e_const);

        if (e_total > 0) {
            res *= seq_pow((seq_t)p, e_total);
        }
    }

    return res;
}
