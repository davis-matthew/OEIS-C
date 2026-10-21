/*
 * A225821  a(n) = Product_{p | p is prime and p, p-1 both divide n}.
 * https://oeis.org/A225821
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product of all primes p such that p | n and p-1 | n
 * Range:     u8 n <= 659, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A225821 */
seq_t a(idx_t n)
{
    /* for safety handle n < 1, though the sequence is defined for n ≥ 1 */
    if (n < 1)
        return 1;

    idx_t m = n;          /* copy for factorisation */
    seq_t res = 1;        /* product accumulator */

    /* trial‑division factorisation of m */
    for (idx_t p = 2; p * p <= m; ++p)
    {
        if (m % p == 0)          /* p is a prime divisor of n */
        {
            if (n % (p - 1) == 0)   /* p-1 also divides n */
                res *= (seq_t)p;    /* multiply into the product */

            while (m % p == 0)      /* remove all powers of p from m */
                m /= p;
        }
    }

    /* if a prime factor > sqrt(n) remains */
    if (m > 1)
    {
        if (n % (m - 1) == 0)
            res *= (seq_t)m;
    }

    return res;
}
