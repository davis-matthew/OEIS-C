/*
 * A134339  a(n) = product of the positive "non-isolated divisors" of (2n). A
 *          divisor, k, of n is non-isolated if (k-1) or (k+1) also divides n.
 * https://oeis.org/A134339
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = product of the positive non‑isolated divisors of 2n
 * Range:     u8 n <= 29, u16 n <= 104, u32 n <= 629, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0) return 1;          /* not used, but safe */

    unsigned long long m = ((unsigned long long)n) * 2ULL;  /* m = 2n */
    seq_t prod = 1;

    for (unsigned long long d = 1; d * d <= m; ++d)
    {
        if (m % d == 0)
        {
            /* divisor d */
            bool nonisol = (d > 1 && (m % (d - 1) == 0)) || (m % (d + 1) == 0);
            if (nonisol)
                prod = prod * (seq_t)d;

            /* paired divisor e = m/d */
            unsigned long long e = m / d;
            if (e != d)
            {
                nonisol = (e > 1 && (m % (e - 1) == 0)) || (m % (e + 1) == 0);
                if (nonisol)
                    prod = prod * (seq_t)e;
            }
        }
    }

    return prod;
}
