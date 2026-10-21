/*
 * A086165  a(n) = |{ (x,y,z) | x < y < z and lcm(x,y,z) = n}|.
 * https://oeis.org/A086165
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (A070919(n) - 3*A048691(n) + 2)/6 where A070919(n) = ∏_p
 *            ((e_p + 1)^3 - e_p^3) A048691(n) = ∏_p ((e_p + 1)^2 - e_p^2)
 *            (e_p is the exponent of prime p in the factorisation of n)
 * Range:     u8 n <= 29, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

/* Simple trial division factorisation.
   Fills arrays pr[], exp[] with the prime factors and their exponents,
   and sets *cnt to the number of distinct primes. */
static void factor_uint64(uint64_t n, uint64_t *pr, uint64_t *exp, size_t *cnt)
{
    size_t c = 0;
    for (uint64_t p = 2; p <= n / p; ++p) {
        if (n % p == 0) {
            pr[c]   = p;
            exp[c]  = 0;
            while (n % p == 0) {
                n   /= p;
                exp[c] += 1;
            }
            ++c;
        }
    }
    if (n > 1) {               /* Remaining prime factor. */
        pr[c]   = n;
        exp[c]  = 1;
        ++c;
    }
    *cnt = c;
}

seq_t a(idx_t n)
{
    /* Factorise n. */
    uint64_t primes[16], exps[16];
    size_t pcnt;
    factor_uint64((uint64_t)n, primes, exps, &pcnt);

    /* Multiplicative accumulation of the terms. */
    seq_t prod_trip = (seq_t)1;   /* A070919(n) */
    seq_t prod_pair = (seq_t)1;   /* A048691(n) */

    for (size_t i = 0; i < pcnt; ++i) {
        uint64_t e = exps[i];
        seq_t ep1   = (seq_t)(e + 1);
        seq_t ep    = (seq_t) e;

        /* ((e+1)^3 - e^3) */
        seq_t term_trip =
                ep1 * ep1 * ep1 - ep * ep * ep;

        /* ((e+1)^2 - e^2) */
        seq_t term_pair =
                ep1 * ep1 - ep * ep;

        prod_trip *= term_trip;
        prod_pair *= term_pair;
    }

    /* a(n) = (prod_trip - 3*prod_pair + 2)/6 */
    seq_t three    = (seq_t)3;
    seq_t two      = (seq_t)2;
    seq_t six      = (seq_t)6;

    seq_t numer = prod_trip - three * prod_pair + two;
    return numer / six;
}
