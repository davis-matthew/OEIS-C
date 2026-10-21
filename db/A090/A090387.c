/*
 * A090387  Numerator of d(n)/n, where d(n) (A000005) is the number of
 *          divisors of n.
 * https://oeis.org/A090387
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, frac, nonn
 * Formula:   a(n) = d(n)/gcd(d(n), n)
 * Range:     u8 n <= 1824, u16 n <= 9395, u32 n <= 9395, u64 n <= 9395,
 *            u128 n <= 9395, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"

/* Return the numerator of d(n)/n, where d(n) is the number of divisors of n.
   The function works for any n >= 1 and may be called in arbitrary order. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* offset starts at 1, guard just in case */

    uint64_t m = (uint64_t)n;        /* work in 64‑bit for factorisation */
    seq_t d = (seq_t)1;              /* divisor count */

    /* factorise m by trial division */
    for (uint64_t p = 2; p <= m / p; ++p)
    {
        if (m % p == 0)
        {
            uint64_t cnt = 0;
            while (m % p == 0)
            {
                m /= p;
                ++cnt;
            }
            d *= (seq_t)(cnt + 1);
        }
    }
    if (m > 1)  /* remaining prime factor */
    {
        d *= (seq_t)2;
    }

    /* reduce the fraction d(n)/n */
    seq_t g = gcd((seq_t)n, d);
    seq_t numer = d / g;
    return numer;
}
