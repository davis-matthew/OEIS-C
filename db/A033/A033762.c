/*
 * A033762  Product t2(q^d); d | 3, where t2 = theta2(q) / (2 * q^(1/4)).
 * https://oeis.org/A033762
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = b(2*n+1) where b is multiplicative: * - b(3^e) = 1, * -
 *            if p ≡ 5 (mod 6): b(p^e) = 1 when e even, else 0, * - otherwise
 *            (p ≡ 1 (mod 6)): b(p^e) = e + 1. * Hence a(0) = 1
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* m = 2*n + 1 – always odd.  Use 128‑bit arithmetic to avoid overflow. */
    idx_t m = (idx_t)((unsigned __int128)2 * n + 1);
    if (m == 1) return (seq_t)1;          /* a(0) = 1 */

    seq_t result = 1;
    idx_t temp = m;

    for (idx_t p = 3; (unsigned __int128)p * p <= temp; p += 2)
    {
        if (temp % p == 0)
        {
            idx_t exp = 0;
            while (temp % p == 0)
            {
                temp /= p;
                exp++;
            }

            if (p == 3)
                continue;                  /* factor 1 for 3^e */

            if (p % 6 == 5)
            {
                if (exp & 1)               /* odd exponent → zero */
                    return (seq_t)0;
                /* even exponent → factor 1, nothing to multiply */
            }
            else
            {
                result *= (seq_t)(exp + 1); /* e+1 for p ≡ 1 (mod 6) */
            }
        }
    }

    /* Handle a remaining prime factor > sqrt(m) */
    if (temp > 1)
    {
        idx_t p = temp;
        if (p == 3)
            /* factor 1, no change */
            ;
        else if (p % 6 == 5)
        {
            /* exponent is 1, odd → zero */
            return (seq_t)0;
        }
        else
        {
            result *= (seq_t)2;            /* exp + 1 = 1 + 1 = 2 */
        }
    }

    return result;
}
