/*
 * A020478  Number of singular 2 X 2 matrices over Z(n) (i.e., with
 *          determinant = 0).
 * https://oeis.org/A020478
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = n^4 - A005353(n) a(p^e) = p^(2*e - 1)*(p^(e+1) + p^e - 1)
 *            (multiplicative)
 * Range:     u8 n <= 5, u16 n <= 35, u32 n <= 1259, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute base^exp as seq_t, exp is idx_t (non‑negative) */
static seq_t pow_seq_idx(seq_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    seq_t cur = base;
    while (exp > 0)
    {
        if (exp & 1) result *= cur;
        exp >>= 1;
        if (exp) cur *= cur;
    }
    return result;
}

/* Contribution of a prime power p^e to a(p^e) */
static seq_t prime_power_contrib(idx_t p, idx_t e)
{
    /* p^(2*e-1) */
    seq_t pow2e_minus1 = pow_seq_idx((seq_t)p, 2*e - 1);
    /* p^e and p^(e+1) */
    seq_t pow_e     = pow_seq_idx((seq_t)p, e);
    seq_t pow_e_next = pow_seq_idx((seq_t)p, e + 1);
    /* p^(e+1) + p^e - 1 */
    seq_t sum = pow_e_next + pow_e - (seq_t)1;
    return pow2e_minus1 * sum;
}

seq_t a(idx_t n)
{
    /* offset 1: a(1)=1, etc. */
    if (n == 0) return (seq_t)0;  /* undefined for 0, but safe */
    seq_t result = (seq_t)1;
    idx_t m = n;

    /* trial division for factorisation */
    for (idx_t p = 2; p * p <= m; ++p)
    {
        if (m % p == 0)
        {
            idx_t e = 0;
            while (m % p == 0)
            {
                m /= p;
                ++e;
            }
            result *= prime_power_contrib(p, e);
        }
    }
    /* remaining prime factor >1 */
    if (m > 1)
    {
        result *= prime_power_contrib(m, 1);
    }

    return result;
}
