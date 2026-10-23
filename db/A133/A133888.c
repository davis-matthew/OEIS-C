/*
 * A133888  Binomial(n+8,n) mod 8.
 * https://oeis.org/A133888
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(n+8,8) mod 8
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Return a(n) = C(n+8,8) mod 8.
   Computation uses exponent of 2 and odd parts modulo 8,
   avoiding large intermediate values. */
seq_t a(idx_t n)
{
    /* m = n + 8, computed in __uint128_t to avoid overflow. */
    __uint128_t m = ( __uint128_t )n + 8;

    unsigned e = 0;           /* exponent of 2 in the binomial */
    unsigned prod = 1;        /* odd part modulo 8 */

    for (idx_t i = 1; i <= 8; ++i)
    {
        /* current numerator: m - (i-1) = n + 9 - i   */
        __uint128_t num = m - (i - 1);
        unsigned denom = (unsigned)i;   /* i is small (1..8) */

        /* Pull out factors of 2 from numerator */
        while ((num & 1) == 0)
        {
            num >>= 1;
            ++e;
        }

        /* Pull out factors of 2 from denominator */
        while ((denom & 1) == 0)
        {
            denom >>= 1;
            if (e > 0)
                --e;
            else
                --e;                 /* e will be non‑negative overall */
        }

        /* Multiply by odd numerator modulo 8 */
        prod = ( prod * (unsigned)(num & 7) ) & 7;

        /* Multiply by inverse of odd denominator modulo 8 */
        unsigned inv;
        switch (denom)
        {
            case 1: inv = 1; break;
            case 3: inv = 3; break;
            case 5: inv = 5; break;
            case 7: inv = 7; break;
            default: inv = 1; break;   /* unreachable because denom is odd */
        }
        prod = ( prod * inv ) & 7;
    }

    /* If exponent of 2 is at least 3, result is 0 mod 8. */
    if (e >= 3)
        return (seq_t)0;

    /* 2^e modulo 8 (e is 0,1,2). */
    unsigned pow2 = 1U << e;
    return (seq_t)( (pow2 * prod) & 7 );
}
