/*
 * A238533  Number of solutions to gcd(x^2 + y^2 + z^2 + t^2 + h^2, n) = 1
 *          with x,y,z,t,h in [0,n-1].
 * https://oeis.org/A238533
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = phi(n^5) = n^4 * phi(n)
 * Range:     u8 n <= 3, u16 n <= 10, u32 n <= 88, u64 n <= 7150,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function uses only integer arithmetic and the types seq_t and idx_t
   supplied by the build. It does not rely on any external utility headers. */

#include <stdint.h>

/* Compute Euler's totient function for a 64‑bit integer n, returning a
   seq_t result. The algorithm uses trial division, which is fast enough
   for n ≤ 10000 (the intended input range). */
static seq_t _phi_u64(uint64_t n)
{
    seq_t result = (seq_t)n;
    uint64_t t = n;
    for (uint64_t p = 2; p * p <= t; ++p)
    {
        if (t % p == 0)
        {
            result -= result / (seq_t)p;
            while (t % p == 0) t /= p;
        }
    }
    if (t > 1) result -= result / (seq_t)t;
    return result;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n = 0, return 0 */
    seq_t phi_n = _phi_u64((uint64_t)n);      /* phi(n) */
    seq_t n4     = (seq_t)n * (seq_t)n * (seq_t)n * (seq_t)n;  /* n^4 */
    return n4 * phi_n;
}
