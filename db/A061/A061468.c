/*
 * A061468  a(n) = d(n) + phi(n), where d(n) is the number of divisors
 *          (A000005) and phi(n) is Euler's totient function (A000010).
 * https://oeis.org/A061468
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = d(n) + φ(n) (number of divisors plus Euler's totient)
 * Range:     u8 n <= 256, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute φ(n) for unsigned 64‑bit integer arguments, returning a seq_t. */
static inline seq_t phi_u64(idx_t n)
{
    uint64_t res = (uint64_t)n;          /* current φ value   */
    uint64_t m   = (uint64_t)n;          /* working copy      */

    for (idx_t p = 2; (uint64_t)p * (uint64_t)p <= m; ++p)
    {
        if (m % p == 0)
        {
            while (m % p == 0)
                m /= p;
            res -= res / p;              /* multiply by (1 – 1/p)  */
        }
    }
    if (m > 1)                            /* final prime factor    */
        res -= res / m;

    return (seq_t)res;
}

/* Compute τ(n), the number of divisors of n, returning a seq_t. */
static inline seq_t tau_u64(idx_t n)
{
    uint64_t res = 1;
    uint64_t m   = (uint64_t)n;

    for (idx_t p = 2; (uint64_t)p * (uint64_t)p <= m; ++p)
    {
        if (m % p == 0)
        {
            uint64_t exp = 0;
            while (m % p == 0)
            {
                m /= p;
                ++exp;
            }
            res *= (exp + 1);            /* multiply by (e+1) */
        }
    }
    if (m > 1)                            /* remaining prime factor  */
        res *= 2;

    return (seq_t)res;
}

/* Required externally linked function for the sequence. */
seq_t a(idx_t n)
{
    return phi_u64(n) + tau_u64(n);
}
