/*
 * A163109  a(n) = phi(tau(n)).
 * https://oeis.org/A163109
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = φ(τ(n)) – Euler’s totient of the number of divisors of n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation factors n by simple trial division (sufficient for n ≤ 10000),
   computes τ(n) and then Euler’s phi function on that value.  No external
   helper functions are required. */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* n is non‑zero by the OEIS offset; cast to uint64_t for factoring. */
    uint64_t nn = (uint64_t)n;
    uint64_t tau = 1;          /* τ(n) */
    uint64_t temp = nn;

    /* Factor n, building τ(n) = ∏(e_i+1). */
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
            tau *= (exp + 1);
        }
    }
    if (temp > 1)                /* remaining prime factor */
        tau *= 2;

    /* Compute φ(tau).  τ is small (≤ 1344 for n ≤ 10000), so trial division is fine. */
    uint64_t m = tau;
    uint64_t phi = m;
    temp = m;
    for (uint64_t p = 2; p * p <= temp; ++p)
    {
        if (temp % p == 0)
        {
            phi -= phi / p;
            while (temp % p == 0)
                temp /= p;
        }
    }
    if (temp > 1)                /* remaining prime divisor */
        phi -= phi / temp;

    return (seq_t)phi;
}
