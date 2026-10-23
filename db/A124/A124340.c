/*
 * A124340  Number of solutions to n = x^2 + 2*y^2 + 4*(T(z) + T(w)) + 1 where
 *          x and y are integers, z and w are nonnegative integers and T(x) =
 *          (x^2+x)/2.
 * https://oeis.org/A124340
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 1552, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   uint_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS Sequence A124340
    Formula: a(n) multiplicative,  
           a(2^e)=2^e,  
           a(p^e) = (p^(e+1)-1)/(p-1)            if p ≡ 1 or 7 (mod 8),  
           a(p^e) = (p^(e+1)+(-1)^e)/(p+1)        if p ≡ 3 or 5 (mod 8). */
#include <stdint.h>
#include "../utils/uint_pow.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;                 /* only defined for n≥1 */

    seq_t result = 1;
    idx_t temp   = n;

    /* factor out powers of 2 */
    idx_t e = 0;
    while ((temp & 1) == 0) {            /* temp % 2 == 0 */
        temp >>= 1;
        ++e;
    }
    if (e != 0)
        result *= uint_pow(2, e);

    /* factor odd primes */
    for (idx_t p = 3; p <= temp / p; p += 2) {
        if (temp % p == 0) {
            idx_t exp = 0;
            while (temp % p == 0) {
                temp /= p;
                ++exp;
            }

            seq_t p_pow = uint_pow((seq_t)p, (idx_t)(exp + 1));
            seq_t term;
            if (p % 8 == 1 || p % 8 == 7) {
                term = (p_pow - 1) / (p - 1);
            } else { /* p ≡ 3 or 5 (mod 8) */
                seq_t numerator = (exp & 1) ? (p_pow - 1) : (p_pow + 1);
                term = numerator / (p + 1);
            }
            result *= term;
        }
    }

    /* remaining prime factor (if any) */
    if (temp > 1) {
        seq_t p = temp;
        seq_t p_pow = uint_pow(p, 2);     /* exp = 1 → exponent + 1 = 2 */
        seq_t term;
        if (p % 8 == 1 || p % 8 == 7) {
            term = (p_pow - 1) / (p - 1);
        } else { /* p ≡ 3 or 5 (mod 8) */
            term = (p_pow - 1) / (p + 1); /* exp=1 → (-1)^exp = -1 */
        }
        result *= term;
    }

    return result;
}
