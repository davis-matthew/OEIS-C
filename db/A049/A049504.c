/*
 * A049504  a(n) = Product_{i = 0..n-1}
 *          ((3*i+1)!*(6*i)!*(2*i)!)/((4*i)!*(4*i+1)!).
 * https://oeis.org/A049504
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = ∏_{i=0}^{n-1} ((3*i+1)! (6*i)! (2*i)!)/((4*i)! (4*i+1)!)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 3, u64 n <= 4, u128 n <= 6,
 *            big any n
 * Helpers:   build_primes_upto, seq_mul, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/build_primes_upto.h"
#include "../utils/seq_mul.h"
#include "../utils/seq_pow.h"

/* Exponent of prime p in n! using Legendre's formula */
static idx_t exp_in_factorial(idx_t n, uint32_t p)
{
    idx_t e = 0;
    while (n) {
        n /= p;
        e += n;
    }
    return e;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    /* Prime list up to 6*n (inclusive) */
    static uint64_t *primes = NULL;
    static size_t prime_cnt = 0;
    static idx_t prime_limit = 0;

    if (prime_limit < 6 * n) {
        if (primes) free(primes);
        uint64_t *tmp = NULL;
        size_t cnt = 0;
        build_primes_upto(6 * n, &tmp, &cnt);
        primes = tmp;
        prime_cnt = cnt;
        prime_limit = 6 * n;
    }

    /* Accumulate signed exponents for all primes */
    int64_t *exp = (int64_t *)calloc(prime_cnt, sizeof(int64_t));
    if (!exp) return (seq_t)0;   /* allocation failure */

    for (idx_t i = 0; i < n; ++i) {
        idx_t three_i_plus1 = 3 * i + 1;
        idx_t six_i         = 6 * i;
        idx_t two_i         = 2 * i;
        idx_t four_i        = 4 * i;
        idx_t four_i_plus1  = 4 * i + 1;

        for (size_t s = 0; s < prime_cnt; ++s) {
            uint32_t p = (uint32_t)primes[s];
            exp[s] += (int64_t)exp_in_factorial(three_i_plus1, p);
            exp[s] += (int64_t)exp_in_factorial(six_i, p);
            exp[s] += (int64_t)exp_in_factorial(two_i, p);
            exp[s] -= (int64_t)exp_in_factorial(four_i, p);
            exp[s] -= (int64_t)exp_in_factorial(four_i_plus1, p);
        }
    }

    /* Build the product from the exponents */
    seq_t result = (seq_t)1;
    for (size_t s = 0; s < prime_cnt; ++s) {
        if (exp[s] > 0) {
            seq_t term = seq_pow(primes[s], (idx_t)exp[s]);
            result = seq_mul(result, term);
        }
        /* Negative exponents should not occur for the whole product;
           if they do, the term would not be an integer. */
    }

    free(exp);
    return result;
}
