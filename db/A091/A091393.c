/*
 * A091393  a(n) = Product_{ p | n } (1 + Legendre(-3,p) ).
 * https://oeis.org/A091393
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = ∏_{p | n} (1 + Legendre(-3, p))
 * Range:     u8 n <= 8711, u16 n <= 8711, u32 n <= 8711, u64 n <= 8711,
 *            u128 n <= 8711, big any n
 * Helpers:   factor_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the OEIS sequence A091393:
 * for each distinct prime divisor p of n:
 *   if p == 3      -> factor = 1  (Legendre(-3,3) = 0)
 *   else if p ≡ 1 (mod 3) -> factor = 2
 *   else                  return 0   (p ≡ 2 (mod 3) gives Legendre = -1)
 * The result is the product of all such factors, with the empty product
 * (i.e. n = 1) equal to 1.
 */

#include <stdbool.h>
#include "../utils/factor_uint64.h"   /* provides factor_uint64 */

seq_t a(idx_t n)
{
    /* Handle the offset: a(1) = 1 */
    if (n == 1)
        return (seq_t)1;

    uint64_t fac[20];
    int exp[20];
    int cnt;
    factor_uint64((uint64_t)n, fac, exp, &cnt);

    seq_t prod = (seq_t)1;
    for (int i = 0; i < cnt; ++i)
    {
        uint64_t p = fac[i];
        if (p == 3)
            continue;           /* factor = 1, no change */
        /* Only primes p ≡ 1 (mod 3) contribute a factor 2 */
        if (p % 3 == 1)
            prod = prod * (seq_t)2;
        else
            return (seq_t)0;    /* factor = 0, product stays 0 */
    }

    return prod;
}
