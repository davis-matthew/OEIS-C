/*
 * A136701  Final nonzero digit of n! in base 15.
 * https://oeis.org/A136701
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = (n! / 15^min(v3(n!), v5(n!))) mod 15 where v3 and v5 are
 *            the exponents of 3 and 5 in n!
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   powmod_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/powmod_int.h"

seq_t a(idx_t n)
{
    seq_t prod = 1;          /* product modulo 15, after stripping all 3's and 5's */
    idx_t cnt3 = 0;         /* total count of factor 3 removed */
    idx_t cnt5 = 0;         /* total count of factor 5 removed */

    for (idx_t i = 1; i <= n; ++i)
    {
        idx_t x = i;
        while (x % 3 == 0)
        {
            x /= 3;
            ++cnt3;
        }
        while (x % 5 == 0)
        {
            x /= 5;
            ++cnt5;
        }
        prod = (prod * (seq_t)(x % 15)) % 15;
    }

    /* remaining unpaired 3's or 5's to multiply back */
    if (cnt3 > cnt5)
    {
        prod = (prod * powmod_int(3, cnt3 - cnt5)) % 15;
    }
    else if (cnt5 > cnt3)
    {
        prod = (prod * powmod_int(5, cnt5 - cnt3)) % 15;
    }

    return prod;
}
