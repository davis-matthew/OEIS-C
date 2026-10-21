/*
 * A077100  a(n) = lcm(A051612(n), A065387(n)), where A051612(n) = sigma(n) -
 *          phi(n) and A065387(n) = sigma(n) + phi(n).
 * https://oeis.org/A077100
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 143, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A077100 – a(n) = lcm( σ(n)−φ(n) , σ(n)+φ(n) )
    σ(n)  = sum of divisors of n
    φ(n)  = Euler's totient of n
    n is 1‑based; the function is defined for all n ≥ 1.  */

/*  The sequence is computed by factoring n (n ≤ 10 000 in the
    tests).  For each prime power pᵉ:
        φ  contribution :  n / p * (p−1)
        σ  contribution :  (p^(e+1)−1)/(p−1)               */
seq_t a(idx_t n)
{
    /*  n is small (≤ 10 000 in the test set) – quick trial division.  */
    uint64_t rem = n;          /* copy of n for factoring */
    seq_t sigma = 1;           /* product of σ‑factors           */
    seq_t phi   = (seq_t)n;    /* start φ(n) = n                 */

    /* factor n */
    for (uint64_t p = 2; p <= rem / p; ++p)
    {
        if (rem % p != 0) continue;

        /* count exponent of prime p */
        uint64_t exp = 0;
        while (rem % p == 0)
        {
            rem /= p;
            ++exp;
        }

        /* φ–contribution */
        phi = (phi / (seq_t)p) * (seq_t)(p - 1);

        /* σ–contribution : (p^(exp+1)−1)/(p−1)  */
        seq_t pow = 1;                  /* p^(exp+1) */
        for (uint64_t i = 0; i < exp + 1; ++i) pow *= (seq_t)p;
        seq_t numerator = pow - 1;
        seq_t denom      = (seq_t)(p - 1);
        seq_t factor     = numerator / denom;
        sigma *= factor;
    }

    /* any remaining prime factor larger than sqrt(original n)  */
    if (rem > 1)
    {
        uint64_t p = rem;
        phi = (phi / (seq_t)p) * (seq_t)(p - 1);

        seq_t numerator = (seq_t)p * (seq_t)p - 1;
        seq_t denom      = (seq_t)(p - 1);
        seq_t factor     = numerator / denom;
        sigma *= factor;
    }

    /* special case n = 1  */
    if (sigma == phi) return 0;    /* σ=φ=1  ⇒  A=B=0  */

    seq_t A = sigma - phi;         /* σ(n)−φ(n) */
    seq_t B = sigma + phi;         /* σ(n)+φ(n) */

    /* gcd(A,B) ----------------------------------------------------- */
    seq_t a_val = A, b_val = B;
    while (b_val != 0)
    {
        seq_t r = a_val % b_val;
        a_val = b_val;
        b_val = r;
    }
    seq_t g = a_val;                /* g = gcd(A,B) */

    /* lcm(A,B) = A/g * B ------------------------------------------ */
    return (A / g) * B;
}
