/*
 * A031367  Inflation orbit counts.
 * https://oeis.org/A031367
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{d|n} mu(d) * (Lucas(n/d) - ( (n/d) mod 2 == 0 ? 2 :
 *            0 )) Here Lucas(k) is the k‑th Lucas number with L(0)=2, L(1)=1,
 *            L(k)=L(k-1)+L(k-2)
 * Range:     u8 n <= 11, u16 n <= 23, u32 n <= 46, u64 n <= 92, u128 n <= 184,
 *            big any n
 * Helpers:   lucas_number, mobius_value
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/lucas_number.h"
#include "../utils/mobius_value.h"

/* ---------------------------------------------- */
/* Helper: compute Möbius value of a small integer */

/* ---------------------------------------------- */
/* Helper: fast computation of Lucas number L(k) */

/* ---------------------------------------------- */
/* Main sequence function                         */
/* ---------------------------------------------- */
seq_t a(idx_t n)
{
    /* Sum over all divisors of n.  We iterate up to sqrt(n). */
    seq_t result = (seq_t)0;

    for (idx_t d = 1; d * d <= n; ++d)
    {
        if (n % d != 0)
            continue;

        idx_t e = n / d;           /* complementary divisor */

        int mu_d = mobius_value(d);
        if (mu_d)
        {
            seq_t term = lucas_number(e);
            if ((e & 1) == 0)      /* even e -> subtract 2 */
                term -= (seq_t)2;
            if (mu_d == 1)
                result += term;
            else
                result -= term;
        }

        if (e != d)               /* handle the paired divisor */
        {
            int mu_e = mobius_value(e);
            if (mu_e)
            {
                seq_t term = lucas_number(d);
                if ((d & 1) == 0)
                    term -= (seq_t)2;
                if (mu_e == 1)
                    result += term;
                else
                    result -= term;
            }
        }
    }

    return result;
}
