/*
 * A162444  Denominators of the BG1[ -5,n] coefficients of the BG1 matrix.
 * https://oeis.org/A162444
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, frac, nonn
 * Formula:   a(n) = denominator of 4^(n-1)*(1-8*n+12*n^2)*(n-1)!^2 / (2*n-2)!
 * Range:     u8 n <= 8, u16 n <= 11, u32 n <= 20, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(1) = 1, a(2) = 1, a(3) = 3, a(4) = 5, … (OEIS A162444) */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

/* Exponent of prime p in m! using Legendre's formula. */
static inline idx_t exp_in_factorial(idx_t m, uint32_t p)
{
    idx_t e = 0;
    uint64_t pp = p;
    while (pp <= m) {
        e += m / pp;
        pp *= p;
    }
    return e;
}

/* Integer power for seq_t, exponent fits in idx_t. */
static inline seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    while (exp) {
        if (exp & 1U)
            result *= base;
        exp >>= 1U;
        if (exp)
            base *= base;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;   /* undefined for n<1; return 0 */

    /* For n = 1 the denominator is 1. */
    if (n == 1)
        return (seq_t)1;

    /* Limit for primes: primes up to 2*n-2 are needed. */
    idx_t limit = 2 * n - 2;

    /* Allocate sieve array. */
    bool *sieve = (bool *)malloc((size_t)(limit + 1));
    if (!sieve)
        return (seq_t)0;   /* allocation failure */

    /* Simple sieve of Eratosthenes. */
    for (idx_t i = 0; i <= limit; ++i)
        sieve[i] = true;
    sieve[0] = sieve[1] = false;
    idx_t sqrt_lim = 0;
    while ((idx_t)(sqrt_lim + 1) * (idx_t)(sqrt_lim + 1) <= limit)
        ++sqrt_lim;
    for (idx_t i = 2; i <= sqrt_lim; ++i) {
        if (sieve[i]) {
            for (idx_t j = i * i; j <= limit; j += i)
                sieve[j] = false;
        }
    }

    /* Collect primes into an array. */
    size_t prime_cap = limit / 2 + 1;   /* upper bound on number of primes */
    uint32_t *primes = (uint32_t *)malloc(prime_cap * sizeof(uint32_t));
    if (!primes) {
        free(sieve);
        return (seq_t)0;   /* allocation failure */
    }
    size_t prime_cnt = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (sieve[i])
            primes[prime_cnt++] = (uint32_t)i;

    /* Free the sieve array: no longer needed. */
    free(sieve);

    /* Polynomial value P = 1 - 8*n + 12*n^2.  Use unsigned 64-bit
       to safely hold the value for n up to 10^4. */
    uint64_t poly = 1ULL + 12ULL * n * n - 8ULL * n;

    seq_t denom = (seq_t)1;

    for (size_t k = 0; k < prime_cnt; ++k) {
        uint32_t p = primes[k];

        /* Exponents in denominator and numerator. */
        idx_t e_d = exp_in_factorial(limit, p);                 /* (2n-2)!   */
        idx_t e_n = 2 * exp_in_factorial(n - 1, p);             /* (n-1)!²  */
        if (p == 2)
            e_n += 2 * (n - 1);                                 /* 4^(n-1)  */

        /* Exponent from polynomial factor. */
        uint64_t tmp = poly;
        idx_t e_poly = 0;
        while (tmp % p == 0) {
            tmp /= p;
            ++e_poly;
        }
        e_n += e_poly;

        /* Remaining exponent for the denominator. */
        if (e_d > e_n) {
            idx_t exp_needed = e_d - e_n;
            denom *= pow_seq((seq_t)p, exp_needed);
        }
    }

    /* Clean up dynamic array. */
    free(primes);

    return denom;
}
