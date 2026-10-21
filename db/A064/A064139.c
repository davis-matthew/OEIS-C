/*
 * A064139  Sum of divisors of central binomial coefficient binomial(n,
 *          floor(n/2)).
 * https://oeis.org/A064139
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = σ(binomial(n,⌊n/2⌋)) – sum of divisors of the central
 *            binomial coefficient
 * Range:     u8 n <= 8, u16 n <= 17, u32 n <= 33, u64 n <= 65, u128 n <= 129,
 *            big any n
 * Helpers:   big_pow, build_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation uses the prime‑exponent formula for the sum of divisors:
   σ(p^e) = (p^(e+1)-1)/(p-1).  To obtain the exponent e of a prime p in
   binomial(n,⌊n/2⌋) we compute the exponent of p in n! minus the
   exponents in ⌊n/2⌋! and ⌈n/2⌉! . */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/build_primes.h"
#include "../utils/big_pow.h"

/* exponent of prime p in n! */
static idx_t factorial_exponent(idx_t n, idx_t p)
{
    idx_t e = 0;
    for (idx_t div = p; div <= n; div *= p)
    {
        e += n / div;
        if (div > n / p)   /* avoid overflow of div *= p */
            break;
    }
    return e;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;            /* undefined offset – safe fallback */

    idx_t k  = n / 2;                /* ⌊n/2⌋ */
    idx_t nk = n - k;                /* ⌈n/2⌉ */

    /* Generate all primes ≤ n */
    idx_t *primes = NULL;
    size_t prime_cnt = 0;
    build_primes(n, &primes, &prime_cnt);

    seq_t result = 1;

    for (size_t i = 0; i < prime_cnt; ++i)
    {
        idx_t p   = primes[i];
        idx_t e   = factorial_exponent(n, p)
                  - factorial_exponent(k,  p)
                  - factorial_exponent(nk, p);
        if (e == 0) continue;        /* prime does not divide the coefficient */

        seq_t exp_seq = (seq_t)(e + 1);                /* for p^(e+1) */
        seq_t pow_val = big_pow((seq_t)p, exp_seq);   /* p^(e+1) */
        seq_t term    = (pow_val - 1) / ((seq_t)p - 1); /* (p^(e+1)-1)/(p-1) */
        result *= term;
    }

    free(primes);
    return result;
}
