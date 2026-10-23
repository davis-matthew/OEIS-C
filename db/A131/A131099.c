/*
 * A131099  a(n) = n times number of divisors of n of form 3m+1 - n times
 *          number of divisors of form 3m+2.
 * https://oeis.org/A131099
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = n * (d_{1 mod 3}(n) – d_{2 mod 3}(n)) Multiplicative with
 *            a(pᵉ) = 3ᵉ if p = 3 (e+1)·pᵉ if p ≡ 1 (mod 3) pᵉ (e even) if p ≡
 *            2 (mod 3) 0 (e odd) if p ≡ 2 (mod 3)
 * Range:     u8 n <= 90, u16 n <= 8280, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* small helper: exponentiation by squaring in seq_t */
static seq_t pow_seq_uint(seq_t base, uint64_t exp)
{
    seq_t res = 1;
    while (exp > 0)
    {
        if (exp & 1ULL)
            res *= base;
        base *= base;
        exp >>= 1ULL;
    }
    return res;
}

/* sequence a(n) for OEIS A131099 */
seq_t a(idx_t n)
{
    if (n == 0)          /* outside offset, safe guard */
        return (seq_t)0;
    uint64_t tn = n;     /* use native type for factorisation */
    seq_t result = 1;    /* multiplicative accumulation */

    /* trial division up to sqrt(tn) avoiding overflow */
    uint64_t p = 2;
    while (p <= tn / p)
    {
        if (tn % p == 0)
        {
            uint64_t e = 0;
            while (tn % p == 0)
            {
                tn /= p;
                ++e;
            }

            seq_t term = 0;
            seq_t p_pow = pow_seq_uint((seq_t)p, e);

            if (p == 3ULL)
                term = p_pow;
            else if (p % 3ULL == 1ULL)
                term = (seq_t)(e + 1ULL) * p_pow;
            else /* p % 3 == 2 */
            {
                if (e & 1ULL)
                    term = 0;
                else
                    term = p_pow;
            }

            result *= term;
            if (result == 0)
                return (seq_t)0;   /* zero propagates */
        }
        ++p;
    }

    /* any remaining prime factor > sqrt(original n) */
    if (tn > 1ULL)
    {
        uint64_t pprime = tn;
        seq_t term = 0;
        seq_t p_pow = (seq_t)pprime;   /* exponent is 1 */

        if (pprime == 3ULL)
            term = p_pow;
        else if (pprime % 3ULL == 1ULL)
            term = (seq_t)(2ULL) * p_pow;
        else /* pprime % 3 == 2 */
            term = 0;          /* exponent 1 is odd */

        result *= term;
        if (result == 0)
            return (seq_t)0;
    }

    return result;
}
