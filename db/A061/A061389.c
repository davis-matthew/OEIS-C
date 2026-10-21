/*
 * A061389  Number of (1+phi)-divisors of n.
 * https://oeis.org/A061389
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = ∏_{p^e || n} (φ(e)+1) where φ is Euler’s totient function
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   totient_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/totient_uint64.h"

seq_t a(idx_t n)
{
    uint64_t val = (uint64_t)n;
    if (val == 0) return (seq_t)1;        /* a(0) is undefined, return 1 here */

    seq_t res = (seq_t)1;
    for (uint64_t p = 2; p * p <= val; ++p)
    {
        if (val % p == 0)
        {
            uint64_t exp = 0;
            while (val % p == 0)
            {
                val /= p;
                ++exp;
            }
            seq_t phi_e = totient_uint64(exp);
            res = res * (phi_e + (seq_t)1);
        }
    }
    if (val > 1) /* remaining prime factor with exponent 1 */
    {
        seq_t phi_e = totient_uint64((uint64_t)1); /* φ(1) = 1 */
        res = res * (phi_e + (seq_t)1);
    }
    return res;
}
