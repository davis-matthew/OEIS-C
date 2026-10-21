/*
 * A092347  a(0)=1; a(n) = sigma_1(n) + sigma_2(n) + sigma_3(n).
 * https://oeis.org/A092347
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1; a(n) = σ₁(n) + σ₂(n) + σ₃(n) for n ≥ 1 * σₖ(n) is the
 *            sum of the k‑th powers of the positive divisors of n
 * Range:     u8 n <= 5, u16 n <= 39, u32 n <= 1535, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* a(0) is defined as 1 */
    if (n == (idx_t)0)
        return (seq_t)1;

    /* Work with a native 64‑bit integer for factorisation. */
    uint64_t m = (uint64_t)n;
    seq_t sum1 = (seq_t)1;   /* product for σ₁ */
    seq_t sum2 = (seq_t)1;   /* product for σ₂ */
    seq_t sum3 = (seq_t)1;   /* product for σ₃ */

    /* Trial division to factorise m.  For n up to about 10000 this is
     * fast enough and uses only the standard arithmetic allowed by the
     * rules.  The loop runs μ(n) ≈ n / log n times in the worst case. */
    uint64_t t = m;
    for (uint64_t p = 2; p * p <= t; ++p)
    {
        if (t % p == 0)
        {
            /* Count the exponent of p. */
            int exp = 0;
            while (t % p == 0)
            {
                t /= p;
                exp++;
            }

            /* Compute the geometric sum for this prime factor. */
            seq_t term1 = (seq_t)1, term2 = (seq_t)1, term3 = (seq_t)1;
            seq_t pk1 = (seq_t)1, pk2 = (seq_t)1, pk3 = (seq_t)1;
            for (int i = 1; i <= exp; ++i)
            {
                pk1 = pk1 * (seq_t)p;                       /* p^i      */
                pk2 = pk2 * (seq_t)p * (seq_t)p;            /* p^(2i)   */
                pk3 = pk3 * (seq_t)p * (seq_t)p * (seq_t)p; /* p^(3i)   */

                term1 += pk1;
                term2 += pk2;
                term3 += pk3;
            }

            sum1 *= term1;
            sum2 *= term2;
            sum3 *= term3;
        }
    }

    /* If anything remains, it is a prime factor > sqrt(m). */
    if (t > 1)
    {
        seq_t p = (seq_t)t;             /* the remaining prime */
        seq_t term1 = (seq_t)1 + p;     /* 1 + p          */
        seq_t term2 = (seq_t)1 + p * p; /* 1 + p^2        */
        seq_t term3 = (seq_t)1 + p * p * p; /* 1 + p^3  */

        sum1 *= term1;
        sum2 *= term2;
        sum3 *= term3;
    }

    return sum1 + sum2 + sum3;
}
