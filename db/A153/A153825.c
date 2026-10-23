/*
 * A153825  Sum of proper divisors minus the number of proper divisors of n!.
 * https://oeis.org/A153825
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = σ(n!) – n! – τ(n!) + 1 (sum of proper divisors of n!
 *            minus number of proper divisors of n!)
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 33,
 *            big any n
 * Helpers:   build_primes_upto, factorial, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/factorial.h"        /* factorial(idx_t) -> seq_t   */
#include "../utils/pow_uint.h"        /* pow_uint(uint64_t, idx_t) -> seq_t */
#include "../utils/build_primes_upto.h" /* build_primes_upto(idx_t, uint64_t **, size_t *) */

seq_t a(idx_t n)
{
    /* Factorial of n as seq_t */
    seq_t fact = factorial(n);

    /* Generate all primes ≤ n */
    uint64_t *primes = NULL;
    size_t prime_cnt = 0;
    build_primes_upto(n, &primes, &prime_cnt);

    /* Products for σ(n!) and τ(n!) */
    seq_t sigma_prod = 1;   /* σ(n!)          */
    seq_t divcnt_prod = 1;  /* τ(n!)          */

    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = primes[i];
        idx_t e = 0;
        idx_t pw = p;

        /* Compute exponent e = Σ floor(n / p^k) */
        while (pw <= n) {
            e += n / pw;
            if (pw > n / p) break;  /* avoid overflow */
            pw *= p;
        }

        /* σ component: (p^(e+1) – 1) / (p – 1) */
        seq_t term = pow_uint(p, e + 1);  /* p^(e+1) */
        term -= 1;
        term /= (seq_t)(p - 1);

        sigma_prod *= term;          /* accumulate σ(n!) */
        divcnt_prod *= (e + 1);      /* accumulate τ(n!) */
    }

    /* Clean up prime list */
    free(primes);

    /* a(n) = σ(n!) – n! – τ(n!) + 1 */
    seq_t result = sigma_prod - fact - divcnt_prod + 1;
    return result;
}
