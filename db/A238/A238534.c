/*
 * A238534  Number of solutions to gcd(u^2 + v^2 + w^2 + x^2 + y^2 + z^2, n) =
 *          1 with u, v, w, x, y, z in [0,n-1].
 * https://oeis.org/A238534
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   multiplicative; a(2^e)=2^(6e-1),
 *            a(p^e)=(p-1)*p^(6e-4)*(p^3-(-1)^(3*(p-1)/2)) for odd prime p
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 40, u64 n <= 1626,
 *            u128 n <= 2000, big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A238534: number of solutions to gcd(u²+v²+w²+x²+y²+z²,n)=1
   with u,v,w,x,y,z ∈ [0,n-1] */
#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset is 1, 0 is undefined but safe */
    uint64_t temp = (uint64_t)n;
    seq_t result = 1;

    /* handle factor 2 */
    uint32_t e = 0;
    while ((temp & 1ULL) == 0) {
        temp >>= 1;
        ++e;
    }
    if (e > 0) {
        result *= seq_pow((seq_t)2, (idx_t)(6 * e - 1));
    }

    /* handle odd primes */
    for (uint64_t p = 3; p * p <= temp; p += 2) {
        if (temp % p == 0) {
            uint32_t pe = 0;
            while (temp % p == 0) {
                temp /= p;
                ++pe;
            }
            seq_t powp   = seq_pow((seq_t)p, (idx_t)(6 * pe - 4));
            seq_t term   = (seq_t)(p - 1);
            term        *= powp;
            int chi      = (p % 4 == 1) ? 1 : -1;
            seq_t part3  = (seq_t)((uint64_t)p * p * p - chi);
            term        *= part3;
            result      *= term;
        }
    }
    /* remaining prime factor > sqrt(n) */
    if (temp > 1) {
        uint64_t p = temp;
        uint32_t pe = 1;
        seq_t powp   = seq_pow((seq_t)p, (idx_t)(6 * pe - 4));
        seq_t term   = (seq_t)(p - 1);
        term        *= powp;
        int chi      = (p % 4 == 1) ? 1 : -1;
        seq_t part3  = (seq_t)((uint64_t)p * p * p - chi);
        term        *= part3;
        result      *= term;
    }
    return result;
}
