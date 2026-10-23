/*
 * A167342  Totally multiplicative sequence with a(p) = p*(p+3) = p^2+3p for
 *          prime p.
 * https://oeis.org/A167342
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) is the totally multiplicative function with a(p) = p*(p+3)
 *            for any prime p. For n with prime factorisation n = ∏p_k^e_k,
 *            a(n) = ∏(p_k*(p_k+3))^e_k
 * Range:     u8 n <= 7, u16 n <= 31, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* small exponentiation for seq_t */
static seq_t pow_seq_t(seq_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    while (exp)
    {
        if (exp & 1U)
            result = result * base;
        base = base * base;
        exp >>= 1U;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n < 1U)
        return (seq_t)0;          /* undefined for n < 1, safety */
    seq_t result = (seq_t)1;
    idx_t temp = n;

    /* factor 2 separately */
    if ((temp & 1U) == 0)
    {
        idx_t cnt = 0;
        while ((temp & 1U) == 0)
        {
            temp >>= 1U;
            ++cnt;
        }
        if (cnt)
        {
            seq_t base = (seq_t)2 * ((seq_t)2 + (seq_t)3);
            result = result * pow_seq_t(base, cnt);
        }
    }

    /* odd prime factors */
    for (idx_t p = 3U; p <= temp / p; p += 2U)
    {
        if (temp % p == 0)
        {
            idx_t cnt = 0;
            while (temp % p == 0)
            {
                temp /= p;
                ++cnt;
            }
            seq_t base = (seq_t)p * ((seq_t)p + (seq_t)3);
            result = result * pow_seq_t(base, cnt);
        }
    }

    /* remaining prime factor > sqrt(n) */
    if (temp > 1U)
    {
        seq_t base = (seq_t)temp * ((seq_t)temp + (seq_t)3);
        result = result * base;  /* exponent is 1 */
    }

    return result;
}
