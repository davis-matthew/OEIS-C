/*
 * A110601  a(n) = phi(n)*tau(n)^2, where phi is Euler's totient function and
 *          tau(n) is the number of divisors of n.
 * https://oeis.org/A110601
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = phi(n) * tau(n)^2 (Euler's totient times the square of
 *            the divisor count)
 * Range:     u8 n <= 19, u16 n <= 479, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Computes the sequence value a(n) for any n >= 1 without external helpers. */
seq_t a(idx_t n)
{
    /* The OEIS offset is 1, so values for n < 1 are irrelevant. */
    if (n < 1) return (seq_t)0;

    idx_t    temp    = n;          /* copy of n for factorisation */
    seq_t    phi_val = (seq_t)n;   /* Euler's totient (to be updated) */
    seq_t    tau_val = (seq_t)1;   /* number of divisors (starting at 1) */

    /* Trial‑division factorisation.  Using p <= temp / p avoids overflow of p*p. */
    for (idx_t p = 2; p <= temp / p; ++p)
    {
        if (temp % p == 0)
        {
            idx_t e = 0;            /* exponent of prime factor p */
            while (temp % p == 0)
            {
                temp /= p;
                ++e;
            }
            /* Update totient and divisor count in one pass. */
            phi_val -= phi_val / (seq_t)p;          /* phi -= phi/p  */
            tau_val *= (seq_t)(e + 1);             /* tau *= (e+1)  */
        }
    }

    /* Remaining prime factor greater than sqrt(original n). */
    if (temp > 1)
    {
        phi_val -= phi_val / (seq_t)temp;   /* phi -= phi/temp  */
        tau_val *= (seq_t)2;                /* tau *= (1+1)     */
    }

    /* a(n) = phi(n) * tau(n)^2 */
    seq_t res = phi_val * tau_val;   /* phi * tau   */
    res *= tau_val;                  /* * tau again */
    return res;
}
