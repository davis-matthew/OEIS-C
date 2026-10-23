/*
 * A023890  Sum of the nonprime divisors of n.
 * https://oeis.org/A023890
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = sum of nonprime divisors of n (i.e. sum of all divisors
 *            of n excluding the prime ones)
 * Range:     u8 n <= 48, u16 n <= 9405, u32 n <= 9405, u64 n <= 9405,
 *            u128 n <= 9405, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Computes the nth term of OEIS A023890. */
seq_t a(idx_t n)
{
    /* n is at least 1 by the contract of the sequence */
    if (n == 1) return (seq_t)1;   /* σ(1)=1, no prime divisors */

    uint64_t n_val = (uint64_t)n;   /* use native 64‑bit for arithmetic */
    uint64_t temp  = n_val;

    seq_t sigma = (seq_t)1;         /* multiplicative identity for σ(n) */
    seq_t primeSum = (seq_t)0;      /* sum of distinct prime divisors */

    /* trial division up to sqrt(n) */
    for (idx_t p = 2; (unsigned __int128)p * p <= n_val; ++p)
    {
        if (temp % p != 0) continue;

        /* exponent of p in n */
        uint64_t exp = 0;
        while (temp % p == 0) {
            temp /= p;
            ++exp;
        }

        /* compute p^(exp+1) */
        seq_t p_pow = (seq_t)1;
        for (idx_t i = 0; i < exp + 1; ++i)
            p_pow *= (seq_t)p;

        /* contribution of p to σ(n): (p^(exp+1)-1)/(p-1) */
        seq_t denom = (seq_t)p - (seq_t)1;
        seq_t part  = (p_pow - (seq_t)1) / denom;
        sigma *= part;

        /* add p to sum of distinct prime divisors */
        primeSum += (seq_t)p;
    }

    /* if a prime factor > sqrt(n) remains */
    if (temp > 1)
    {
        seq_t p = (seq_t)temp;
        /* for e = 1: part = p+1 */
        sigma *= (p + (seq_t)1);
        primeSum += p;
    }

    return sigma - primeSum;   /* sum of nonprime divisors */
}
