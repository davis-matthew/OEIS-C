/*
 * A229340  Euler totient function of the arithmetic derivative of n: a(n) =
 *          phi(n'), a(1) = 0.
 * https://oeis.org/A229340
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = φ(A003415(n)) (Euler totient of the arithmetic
 *            derivative)
 * Range:     u8 n <= 191, u16 n <= 9364, u32 n <= 9364, u64 n <= 9364,
 *            u128 n <= 9364, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute φ(m) for m up to 64‑bit, returning a seq_t */
static seq_t phi_uint64(uint64_t m)
{
    if (m == 0)
        return 0;   /* not encountered for n > 1 */
    uint64_t result = m;
    uint64_t t = m;
    for (uint64_t p = 2; p * p <= t; ++p)
    {
        if (t % p == 0)
        {
            /* remove all p factors */
            while (t % p == 0)
                t /= p;
            /* φ(m) *= (1 - 1/p) */
            result -= result / p;
        }
    }
    if (t > 1)               /* remaining prime factor > sqrt(t) */
        result -= result / t;
    return (seq_t)result;
}

/* Euler totient function of the arithmetic derivative a(n) */
seq_t a(idx_t n)
{
    if (n == 1)              /* a(1) is defined by convention */
        return 0;

    uint64_t temp = n;          /* copy for factorisation */
    uint64_t deriv = 0;         /* derivative n′ */
    uint64_t original = n;      /* needed for n/p computations */

    /* factorise n using trial division */
    for (uint64_t p = 2; p * p <= temp; ++p)
    {
        if (temp % p == 0)
        {
            uint64_t exp = 0;
            while (temp % p == 0)
            {
                temp /= p;
                ++exp;
            }
            deriv += exp * (original / p);
        }
    }
    /* any remaining prime factor larger than sqrt(original) */
    if (temp > 1)
        deriv += original / temp;   /* exp = 1 */

    /* φ(deriv) */
    seq_t result = phi_uint64(deriv);
    return result;
}
