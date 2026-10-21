/*
 * A055210  Sum of totients of square divisors of n.
 * https://oeis.org/A055210
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = Sum_{d is square | d|n} φ(d) = Π_{p^e||n}
 *            (p^(e+1)+1)/(p+1) for even e, (p^e+1)/(p+1) for odd e
 * Range:     u8 n <= 48, u16 n <= 7920, u32 n <= 9158, u64 n <= 9158,
 *            u128 n <= 9158, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Includes */
#include <stdint.h>
#include <stddef.h>

/* Fast exponentiation for seq_t (unsigned integer type) */
static seq_t pow_seq_t(seq_t base, idx_t exp)
{
    seq_t result = 1;
    while (exp)
    {
        if (exp & 1) result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

/* Simple trial division factorization: fills pr[] with primes, exp[] with exponents,
   and returns the number of distinct prime factors in *cnt. */
static void factor_u64(uint64_t n, uint64_t *pr, uint64_t *exp, size_t *cnt)
{
    *cnt = 0;
    uint64_t p = 2;
    while (p * p <= n)
    {
        if (n % p == 0)
        {
            uint64_t e = 0;
            while (n % p == 0)
            {
                n /= p;
                ++e;
            }
            pr[(*cnt)++] = p;
            exp[(*cnt - 1)] = e;
        }
        p = (p == 2) ? 3 : p + 2;   /* after 2 skip even numbers */
    }
    if (n > 1)
    {
        pr[(*cnt)++] = n;
        exp[(*cnt - 1)] = 1;
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* undefined for n=0, return 0 */

    /* Factorization of n */
    uint64_t pr[64];
    uint64_t exp[64];
    size_t cnt;
    factor_u64((uint64_t)n, pr, exp, &cnt);

    seq_t result = 1;
    for (size_t i = 0; i < cnt; ++i)
    {
        seq_t p = (seq_t)pr[i];
        idx_t e  = exp[i];

        seq_t denom = p + 1U;
        seq_t num;
        if (e & 1U)                 /* odd exponent */
        {
            seq_t pw = pow_seq_t(p, e);
            num = pw + 1U;
        }
        else                         /* even exponent */
        {
            seq_t pw = pow_seq_t(p, e + 1U);
            num = pw + 1U;
        }
        seq_t contrib = num / denom;   /* exact division */
        result *= contrib;
    }
    return result;
}
