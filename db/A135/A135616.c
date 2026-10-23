/*
 * A135616  Number of permutations p of {1,2,...,n} such that p(x) is a
 *          polynomial in x, modulo n, of degree at most 2, for x=1,2,3,...,n.
 * https://oeis.org/A135616
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏_{p^e‖n} f(p,e) , with * f(2,1) = 2 * f(2,e) = 2^{3e-3}
 *            for e ≥ 2 * f(p,e) = p^{3e-2}·(p-1) for odd prime p * The empty
 *            product (n = 1) equals 1
 * Range:     u8 n <= 15, u16 n <= 40, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t result = 1;
    while (exp)
    {
        if (exp & 1)
            result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n <= 1)
        /* a(1)=1, values for n<1 are unused */
        return (seq_t)1;

    seq_t result = 1;
    idx_t m = n;

    for (idx_t p = 2; p <= m / p; ++p)
    {
        if (m % p == 0)
        {
            idx_t e = 0;
            while (m % p == 0)
            {
                m /= p;
                ++e;
            }

            if (p == 2)
            {
                if (e == 1)
                    result *= (seq_t)2;
                else
                    result *= pow_seq((seq_t)2, (idx_t)(3 * e - 3));
            }
            else
            {
                seq_t term = pow_seq((seq_t)p, (idx_t)(3 * e - 2));
                term *= (seq_t)(p - 1);
                result *= term;
            }
        }
    }

    /* Remaining prime factor (if any) has exponent 1 */
    if (m > 1)
    {
        idx_t p = m;
        if (p == 2)
            result *= (seq_t)2;
        else
        {
            seq_t term = (seq_t)p * (seq_t)(p - 1);
            result *= term;
        }
    }

    return result;
}
