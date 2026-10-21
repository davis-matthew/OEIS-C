/*
 * A229179  Number of solutions of x^2 + y^2 + z^2 == -1 (mod n) with x, y,
 *          and z in 0..n-1.
 * https://oeis.org/A229179
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = ∏ a(p^k) for the prime powers p^k in n, * where a(8*m)=0,
 *            a(2)=4, a(4)=8, * and for odd primes p: a(p^k)=p^(2*k-1)*(p+1)
 * Range:     u8 n <= 14, u16 n <= 209, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Fast power for seq_t: base^exp.  exp is an idx_t. */
static seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t res = 1;
    while (exp)
    {
        if (exp & 1U)
            res *= base;
        base *= base;
        exp >>= 1;
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n < 1U)
        return 0;
    if (n == 1U)
        return 1;

    /* If 8 | n, the answer is 0 (multiplicativity with a(8)=0). */
    if ((n & 7) == 0U)  /* n % 8 == 0 */
        return 0;

    seq_t result = 1;
    idx_t temp = n;
    idx_t e = 0;

    /* Count power of 2. */
    while ((temp & 1U) == 0U)
    {
        temp >>= 1;
        ++e;
    }
    if (e == 1U)
        result *= 4;
    else if (e == 2U)
        result *= 8;
    else if (e >= 3U)
        return 0;   /* already handled by n%8==0, but safe guard */

    /* Factor odd primes by trial division. */
    for (idx_t p = 3; p <= temp / p; p += 2U)
    {
        if (temp % p == 0U)
        {
            idx_t k = 0U;
            while (temp % p == 0U)
            {
                temp /= p;
                ++k;
            }
            seq_t term = pow_seq((seq_t)p, 2 * k - 1U) * ((seq_t)p + 1U);
            result *= term;
        }
    }

    /* If a prime factor remains > 1. */
    if (temp > 1U)
    {
        idx_t k = 1U;
        seq_t term = pow_seq((seq_t)temp, 2 * k - 1U) * ((seq_t)temp + 1U);
        result *= term;
    }

    return result;
}
