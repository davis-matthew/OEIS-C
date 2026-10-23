/*
 * A106402  Expansion of eta(q^3)^9 / eta(q)^3 in powers of q.
 * https://oeis.org/A106402
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = multiplicative a(p^e) = ((p^2)^(e+1) - u^(e+1)) / (p^2 -
 *            u), * where u = 0 if p ≡ 0 (mod 3), 1 if p ≡ 1 (mod 3), -1 if p
 *            ≡ 2 (mod 3). * a(1) = 1. This implements OEIS sequence A106402
 * Range:     u8 n <= 4, u16 n <= 16, u32 n <= 1613, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute base^exp (exp small, base fits in seq_t). */
static seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t result = 1;
    while (exp)
    {
        if (exp & 1)
            result *= base;
        exp >>= 1;
        if (exp)
            base *= base;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n == 1)
        return 1;          /* a(1) */

    seq_t result = 1;
    idx_t temp = n;

    /* Trial division factorisation */
    for (idx_t p = 2; p * p <= temp; ++p)
    {
        if (temp % p == 0)
        {
            /* exponent of prime p */
            idx_t e = 0;
            while (temp % p == 0)
            {
                temp /= p;
                ++e;
            }

            /* compute factor for p^e */
            seq_t p2 = (seq_t)p * p;                /* p^2 */
            seq_t num = pow_seq(p2, e + 1);          /* (p^2)^(e+1) */

            int u;
            switch (p % 3)
            {
                case 0:  u = 0;  break;
                case 1:  u = 1;  break;
                default: u = -1; break;
            }

            /* subtract u^(e+1) correctly */
            if (u == 1)
            {
                /* u^k = 1 */
                num -= 1;
            }
            else if (u == -1)
            {
                /* u^k = (-1)^(k) */
                if ((e + 1) & 1)
                    /* subtract (-1) => add 1 */
                    num += 1;
                else
                    /* subtract 1 */
                    num -= 1;
            }
            /* u == 0: nothing to subtract */

            seq_t denom = p2 - (seq_t)u;            /* p^2 - u */
            seq_t factor = num / denom;             /* exact division */

            result *= factor;
        }
    }

    /* Any remaining prime factor > sqrt(n) */
    if (temp > 1)
    {
        idx_t p = temp;
        idx_t e = 1;             /* only one power left */

        seq_t p2 = (seq_t)p * p;
        seq_t num = pow_seq(p2, e + 1);  /* (p^2)^2 */

        int u;
        switch (p % 3)
        {
            case 0:  u = 0;  break;
            case 1:  u = 1;  break;
            default: u = -1; break;
        }

        if (u == 1)
            num -= 1;
        else if (u == -1)
            if ((e + 1) & 1)   /* odd => add 1 */
                num += 1;
            else
                num -= 1;

        seq_t denom = p2 - (seq_t)u;
        seq_t factor = num / denom;

        result *= factor;
    }

    return result;
}
