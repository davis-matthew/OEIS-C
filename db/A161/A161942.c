/*
 * A161942  Odd part of sum of divisors of n.
 * https://oeis.org/A161942
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, mult, nonn
 * Formula:   a(n) = odd part of σ(n) where σ(n) = ∑_{d|n} d (sum of divisors)
 * Range:     u8 n <= 16, u16 n <= 1008, u32 n <= 9416, u64 n <= 9416,
 *            u128 n <= 9416, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

#define ONE ((seq_t)1)

/* Computes the n-th term of OEIS A161942. */
seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 1.  For safety, return 1 for n = 0. */
    if (n == 0) return ONE;

    /* Factor n by trial division.  For the small n discussed (<10 000) this
       is fast enough.  The factorisation is stored in parallel arrays. */
    idx_t tmp = n;
    idx_t primes[32];
    size_t exps[32];
    size_t pcnt = 0;

    for (idx_t p = 2; p * p <= tmp; ++p) {
        if (tmp % p == 0) {
            size_t e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++e;
            }
            primes[pcnt] = p;
            exps[pcnt]   = e;
            ++pcnt;
        }
    }
    if (tmp > 1) {          /* remainder is a prime factor > sqrt(original n) */
        primes[pcnt] = tmp;
        exps[pcnt]   = 1;
        ++pcnt;
    }

    /* Compute σ(n) = ∏_{p^e||n} (p^(e+1)‑1)/(p‑1).  All intermediate
       values fit in seq_t (arbitrary‑precision for the big‑integer build). */
    seq_t sigma = ONE;
    for (size_t i = 0; i < pcnt; ++i) {
        seq_t p = (seq_t)primes[i];
        size_t e = exps[i];

        /* Compute p^(e+1). */
        seq_t p_pow = ONE;
        for (size_t k = 0; k < e + 1; ++k) p_pow *= p;

        /* Numerator of the factor: p^(e+1) - 1. */
        seq_t num = p_pow - ONE;

        /* Denominator: p - 1.  For p = 2 this is 1. */
        seq_t den = p - ONE;

        /* Integer division; result is guaranteed to be integral. */
        seq_t term = num / den;

        sigma *= term;
    }

    /* Extract the odd part by discarding all factors of 2. */
    while ((sigma & ONE) == 0) sigma >>= 1;

    return sigma;
}
