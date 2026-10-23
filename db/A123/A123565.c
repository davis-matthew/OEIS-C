/*
 * A123565  a(n) is the number of positive integers k which are <= n and where
 *          k, k-1 and k+1 are each coprime to n.
 * https://oeis.org/A123565
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, look
 * Formula:   a(n) is multiplicative. For n≥1, a(1) = 1; if n is divisible by
 *            2 or 3 then a(n) = 0; otherwise for an odd prime power p^e,
 *            a(p^e) = (p-3) * p^(e-1)
 * Range:     u8 n <= 262, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 1) return 1;

    seq_t result = 1;
    idx_t m = n;

    for (idx_t p = 2; p * p <= m; ++p)
    {
        if (m % p == 0)
        {
            /* If 2 or 3 divides n, the result is zero. */
            if (p == 2 || p == 3) return 0;

            /* Count exponent of p. */
            idx_t e = 0;
            while (m % p == 0)
            {
                m /= p;
                ++e;
            }

            /* Compute (p-3) * p^(e-1) and multiply into result. */
            seq_t pow = 1;
            seq_t ps  = (seq_t)p;
            for (idx_t i = 1; i < e; ++i) pow *= ps;
            seq_t term = (ps - (seq_t)3) * pow;
            result *= term;
        }
    }

    /* m now holds a remaining prime factor (if any). */
    if (m > 1)
    {
        if (m == 2 || m == 3) return 0;
        seq_t ps   = (seq_t)m;
        seq_t term = (ps - (seq_t)3);          /* exponent is 1 */
        result *= term;
    }

    return result;
}
