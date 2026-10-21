/*
 * A056610  Quotient: squarefree kernel of lcm(1,..,n) (or of n!) divided by
 *          kernel of central binomial coefficient.
 * https://oeis.org/A056610
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = rad(n!) / rad(binomial(n, ⌊n/2⌋)) (rad = squarefree
 *            kernel)
 * Range:     u8 n <= 24, u16 n <= 44, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   build_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/build_primes.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;  /* undefined for n<1; return 0 for safety */

    /* Cache primes up to the maximum n encountered. */
    static idx_t *primes = NULL;
    static size_t prime_cnt = 0;
    static idx_t prime_limit = 0;

    if (n > prime_limit) {
        if (primes) free(primes);
        build_primes(n, &primes, &prime_cnt);
        prime_limit = n;
    }

    seq_t result = 1;

    /* Determine central binomial parameters */
    idx_t k = n >> 1;             /* floor(n/2)          */
    idx_t n_minus_k = n - k;      /* ceil(n/2)           */

    for (size_t i = 0; i < prime_cnt; ++i) {
        idx_t p = primes[i];

        /* exponent of p in n!  */
        idx_t exp_n = 0, power = p;
        while (power <= n) {
            exp_n += n / power;
            if (power > n / p) break;
            power *= p;
        }

        /* exponent of p in k!  */
        idx_t exp_k = 0; power = p;
        while (power <= k) {
            exp_k += k / power;
            if (power > k / p) break;
            power *= p;
        }

        /* exponent of p in (n−k)! */
        idx_t exp_nk = 0; power = p;
        while (power <= n_minus_k) {
            exp_nk += n_minus_k / power;
            if (power > n_minus_k / p) break;
            power *= p;
        }

        /* exponent in the central binomial coefficient */
        idx_t exp_bin = exp_n - exp_k - exp_nk;

        /* If the exponent is 0, the prime does NOT divide the binomial
           coefficient, so it stays in the quotient. */
        if (exp_bin == 0) {
            result *= (seq_t)p;
        }
    }

    return result;
}
