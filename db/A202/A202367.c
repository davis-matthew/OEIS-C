/*
 * A202367  LCM of denominators of the coefficients of polynomials Q^(2)_m(n)
 *          defined by the recursion Q^(2)_0(n)=1; for m >= 1, Q^(2)_m(n) =
 *          Sum_{i=1..n} i^2*Q^(2)_(m-1)(i).
 * https://oeis.org/A202367
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{primes p} p^( Sum_{k>=0}
 *            floor((n-1)/(ceil((p-1)/2)*p^k)) )
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 9, u128 n <= 15,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/big_pow.h"


/* OEIS A202367: LCM of denominators of coefficients of the polynomials
 * Q^(2)_m(n). This implementation follows the conjectured product formula
 * and works for any n >= 1. */
seq_t a(idx_t n)
{
    if (n <= 1) {
        return (seq_t)1;
    }

    idx_t limit = 2 * n - 1;
    /* Simple sieve of Eratosthenes up to 2n-1. */
    bool *is_prime = (bool *)malloc((size_t)(limit + 1));
    if (!is_prime) {
        return (seq_t)0; /* allocation failure – unlikely for the used ranges */
    }
    for (idx_t i = 2; i <= limit; ++i) {
        is_prime[i] = true;
    }
    for (idx_t p = 2; p * p <= limit; ++p) {
        if (is_prime[p]) {
            for (idx_t k = p * p; k <= limit; k += p) {
                is_prime[k] = false;
            }
        }
    }

    seq_t ans = (seq_t)1;
    idx_t n_minus_1 = n - 1;

    for (idx_t p = 2; p <= limit; ++p) {
        if (!is_prime[p]) continue;

        idx_t t   = (p == 2) ? (idx_t)1 : (p - 1) / 2;   /* t = ceil((p-1)/2) */
        idx_t power = 1;
        idx_t exp_sum = 0;

        while (power <= n_minus_1 / t) {
            /* Compute floor((n-1)/(t*power)) without overflow */
            idx_t denom = t * power;            /* fits in idx_t because denom <= n-1 */
            exp_sum += n_minus_1 / denom;

            if (power > n_minus_1 / p) break;   /* prevent overflow on next power */
            power *= p;
        }

        seq_t term = big_pow((seq_t)p, (seq_t)exp_sum);
        ans *= term;
    }

    free(is_prime);
    return ans;
}
