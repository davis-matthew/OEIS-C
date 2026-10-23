/*
 * A011785  Number of 3 X 3 matrices whose determinant is 1 mod n.
 * https://oeis.org/A011785
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = ∏_{p^e∥n} p^(8e-5) * (p^3-1) * (p^2-1) (multiplicative)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 16, u64 n <= 256, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* fast power: computes base^exp for seq_t base and idx_t exponent */
static seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    while (exp)
    {
        if (exp & 1)
            result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

/* a(idx_t n) – number of 3×3 matrices with determinant ≡ 1 (mod n) */
seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)1;

    seq_t res = (seq_t)1;
    idx_t m = n;

    /* trial division up to sqrt(m) */
    for (idx_t p = 2; p * p <= m; ++p)
    {
        if (m % p != 0)
            continue;

        /* determine exponent e of p in n */
        idx_t e = 0;
        while (m % p == 0)
        {
            m /= p;
            ++e;
        }

        seq_t p_seq = (seq_t)p;
        seq_t p2 = p_seq * p_seq;          /* p^2 */
        seq_t p3 = p2 * p_seq;             /* p^3 */
        seq_t mult_part = (p3 - (seq_t)1) * (p2 - (seq_t)1);

        idx_t exp = 8 * e - 5;              /* exponent for p^(8e-5) */
        seq_t pow_part = pow_seq(p_seq, exp);

        res *= pow_part * mult_part;
    }

    /* remaining prime factor, if any (exponent 1) */
    if (m > 1)
    {
        seq_t p_seq = (seq_t)m;
        seq_t p2 = p_seq * p_seq;
        seq_t p3 = p2 * p_seq;
        seq_t mult_part = (p3 - (seq_t)1) * (p2 - (seq_t)1);
        seq_t pow_part = pow_seq(p_seq, 3);    /* 8*1-5 = 3 */

        res *= pow_part * mult_part;
    }

    return res;
}
