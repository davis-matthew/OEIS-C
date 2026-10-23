/*
 * A152762  Sum of proper divisors of Catalan number A000108(n).
 * https://oeis.org/A152762
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = sigma(Catalan(n)) - Catalan(n) (proper divisor sum)
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 19, u64 n <= 35, u128 n <= 67,
 *            big any n
 * Helpers:   build_primes_upto, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/build_primes_upto.h"
#include "../utils/pow_uint.h"

seq_t a(idx_t n) {
    /* Special cases n=0,1 where Catalan=1, sigma(1)=1 => result 0 */
    if (n <= 1) return (seq_t)0;

    /* Generate list of primes up to 2*n */
    uint64_t *primes = NULL;
    size_t prime_cnt = 0;
    build_primes_upto(2 * n, &primes, &prime_cnt);

    seq_t cat = (seq_t)1;          /* Catalan(n) product */
    seq_t sigma = (seq_t)1;        /* sigma(Catalan(n)) product */

    for (size_t idx = 0; idx < prime_cnt; ++idx) {
        uint64_t p = primes[idx];

        /* Exponent of p in binomial(2n,n) */
        idx_t e_bin = 0;
        idx_t pow = p;
        while (pow <= 2 * n) {
            e_bin += (2 * n) / pow - 2 * (n / pow);
            pow *= p;
        }

        /* Exponent of p in n+1 */
        idx_t exp_n1 = 0;
        idx_t temp = n + 1;
        while (temp % p == 0) {
            ++exp_n1;
            temp /= p;
        }

        /* Exponent in Catalan(n) */
        idx_t e_c = e_bin - exp_n1;
        if (e_c == 0) continue; /* trivial prime factor */

        seq_t epow = pow_uint(p, e_c);
        cat *= epow;

        /* Sigma term: (p^{e+1}-1)/(p-1) */
        seq_t numer = epow * (seq_t)p - (seq_t)1;
        seq_t denom = (seq_t)(p - 1);
        seq_t term = numer / denom;
        sigma *= term;
    }

    /* Free primes array */
    free(primes);

    return sigma - cat;
}
