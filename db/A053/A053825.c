/*
 * A053825  Dirichlet inverse of sigma_3 function (A001158).
 * https://oeis.org/A053825
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult
 * Formula:   a(n) = Sum_{d|n} mu(n/d)*mu(d)*d^3 Equivalent multiplicative
 *            formula: a(p^e) = -1-p^3 (e=1), p^3 (e=2), 0 (e>=3) The function
 *            computes a(n) for any n >= 1 using prime factorisation
 * Range:     i8 n <= 5, i16 n <= 32, i32 n <= 1229, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

static size_t factorize(uint64_t n, uint64_t primes[], int exps[])
{
    size_t cnt = 0;
    uint64_t m = n;
    for (uint64_t p = 2; p <= m / p; ++p) {
        if (m % p == 0) {
            int e = 0;
            while (m % p == 0) {
                m /= p;
                ++e;
            }
            primes[cnt]   = p;
            exps[cnt]     = e;
            ++cnt;
        }
    }
    if (m > 1) {
        primes[cnt] = m;
        exps[cnt]   = 1;
        ++cnt;
    }
    return cnt;
}

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;

    uint64_t nn = (uint64_t)n;
    uint64_t primes[64];
    int exps[64];
    size_t pcnt = factorize(nn, primes, exps);

    seq_t res = (seq_t)1;
    for (size_t i = 0; i < pcnt; ++i) {
        int e   = exps[i];
        uint64_t p = primes[i];
        if (e >= 3)
            return (seq_t)0;

        seq_t pp  = (seq_t)p;
        seq_t p3  = pp * pp * pp;
        seq_t factor;

        if (e == 1)
            factor = -(seq_t)1 - p3;
        else   /* e == 2 */
            factor = p3;

        res *= factor;
    }
    return res;
}
