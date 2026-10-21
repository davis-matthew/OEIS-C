/*
 * A211991  Difference between the arithmetic derivative of n and the sum of
 *          proper divisors of n.
 * https://oeis.org/A211991
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = A003415(n) - A001065(n)
 * Range:     i8 n <= 12, i16 n <= 698, i32 n <= 9819, i64 n <= 9819,
 *            i128 n <= 9819, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Arithmetic derivative minus sum of proper divisors */

#include <stdint.h>
#include <stddef.h>

/* Computes the n-th term of OEIS sequence A211991 */
seq_t a(idx_t n)
{
    if (n == 0) /* undefined for n=0, return 0 to avoid UB */
        return (seq_t)0;

    /* Factorisation of n */
    uint64_t temp = n;
    /* We never expect more than 64 distinct prime factors for the sizes considered */
    uint64_t primes[64];
    unsigned int exps[64];
    size_t np = 0;

    while (temp > 1) {
        uint64_t p;
        if ((temp & 1) == 0) {                /* divisible by 2 */
            p = 2;
        } else {
            uint64_t i;
            p = temp;                         /* assume prime until proven composite */
            for (i = 3; i <= temp / i; i += 2) {
                if (temp % i == 0) {
                    p = i;
                    break;
                }
            }
        }

        unsigned int e = 0;
        while (temp % p == 0) {
            temp /= p;
            ++e;
        }
        primes[np] = p;
        exps[np]   = e;
        ++np;
    }

    /* Arithmetic derivative: sum e_i * (n / p_i) */
    seq_t deriv = 0;
    for (size_t j = 0; j < np; ++j) {
        deriv += (seq_t)exps[j] * (((seq_t)n) / (seq_t)primes[j]);
    }

    /* Sum of all divisors σ(n) = ∏ (p_i^(e_i+1)−1)/(p_i−1) */
    seq_t sigma = 1;
    for (size_t j = 0; j < np; ++j) {
        seq_t pow = 1;
        for (int k = 0; k < (int)exps[j] + 1; ++k)
            pow *= (seq_t)primes[j];
        seq_t numerator   = pow - 1;
        seq_t denominator = (seq_t)primes[j] - 1;
        seq_t factor      = numerator / denominator;  /* exact division */
        sigma *= factor;
    }

    /* Sum of proper divisors = σ(n) − n */
    seq_t proper = sigma - (seq_t)n;

    /* Final term: arithmetic derivative minus proper divisor sum */
    return deriv - proper;
}
