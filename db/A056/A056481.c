/*
 * A056481  Number of primitive (aperiodic) palindromic structures using
 *          exactly two different symbols.
 * https://oeis.org/A056481
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 34, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   g
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   OEIS A056481: Number of primitive (aperiodic) palindromic
   structures using exactly two different symbols.
   Formula implemented:
        a(n) = Σ_{d|n} μ(d) · ( 2^{⌈(n/d)/2⌉–1} – 1 )
   where μ is the Möbius function.
*/
#include <stdint.h>
#include <stddef.h>
#include "../utils/g.h"

/* Return the Möbius value of m: 1, -1, or 0. */
static int mobius(idx_t m)
{
    if (m == 1) return 1;
    int cnt = 0;
    for (idx_t p = 2; p * p <= m; ++p)
    {
        if (m % p == 0)
        {
            int exp = 0;
            while (m % p == 0)
            {
                m /= p;
                ++exp;
                if (exp > 1) return 0;      /* square factor → μ=0 */
            }
            ++cnt;                         /* prime factor counted once */
        }
    }
    if (m > 1) ++cnt;                       /* last prime factor */
    return (cnt % 2 == 0) ? 1 : -1;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;           /* offset 0 */

    seq_t sum = (seq_t)0;
    for (idx_t d = 1; d * d <= n; ++d)
    {
        if (n % d != 0) continue;

        int mu_d = mobius(d);
        if (mu_d != 0)
        {
            seq_t term = g(n / d);
            sum += (mu_d == 1) ? term : -term;
        }

        idx_t e = n / d;
        if (e == d) continue;              /* same divisor counted only once */

        int mu_e = mobius(e);
        if (mu_e != 0)
        {
            seq_t term = g(d);
            sum += (mu_e == 1) ? term : -term;
        }
    }
    return sum;
}
