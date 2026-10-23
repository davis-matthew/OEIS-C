/*
 * A138483  Expansion of (phi(q)^3 * phi(q^5) - phi(q) * phi(q^5)^3) / 4 in
 *          powers of q where phi() is a Ramanujan theta function.
 * https://oeis.org/A138483
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) is multiplicative with * a(2^e) = (2^(e+1) - 5*(-1)^e) / 3
 *            (e > 0) * a(5^e) = 5^e * a(p^e) = (p^(e+1)-1) / (p-1) if p ≡ 1,4
 *            (mod 5) * = (p^(e+1)+(-1)^e) / (p+1) if p ≡ 2,3 (mod 5)
 * Range:     u8 n <= 16, u16 n <= 4002, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the value a(n) for index n of OEIS sequence A138483.   */
#include <stdint.h>
#include <stddef.h>

/* Fast integer power with seq_t base and idx_t exponent. */
static inline seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t result = 1;
    while (exp)
    {
        if (exp & 1U)
            result *= base;
        base *= base;
        exp >>= 1U;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* outside OEIS offset, safe guard */

    seq_t result = 1;
    idx_t temp = n;

    /* Factorization by trial division up to sqrt(temp) */
    for (idx_t i = 2; i <= temp / i; ++i)
    {
        if (temp % i != 0) continue;

        /* Count exponent of prime i */
        idx_t exp = 0;
        while (temp % i == 0)
        {
            temp /= i;
            ++exp;
        }

        seq_t term;
        if (i == 2U)
        {
            seq_t pow = pow_seq((seq_t)2U, exp + 1);
            term = (exp & 1U) ? (pow + 5) / 3 : (pow - 5) / 3;
        }
        else if (i == 5U)
        {
            term = pow_seq((seq_t)5U, exp);
        }
        else
        {
            seq_t pow = pow_seq((seq_t)i, exp + 1);
            uint64_t mod5 = i % 5U;
            if (mod5 == 1U || mod5 == 4U)
            {
                term = (pow - 1) / ((seq_t)i - 1);
            }
            else    /* mod5 == 2 or 3 */
            {
                seq_t numerator = pow;
                numerator += (exp & 1U) ? ((seq_t) -1) : 1;
                term = numerator / ((seq_t)i + 1);
            }
        }
        result *= term;
    }

    /* Remaining prime factor > sqrt(original n) */
    if (temp > 1U)
    {
        idx_t p = temp;
        idx_t exp = 1U;

        seq_t term;
        if (p == 2U)
        {
            seq_t pow = pow_seq((seq_t)2U, exp + 1); /* exp==1 */
            term = (pow + 5) / 3;                  /* exp odd -> (-1)^1 = -1 */
        }
        else if (p == 5U)
        {
            term = 5U;   /* 5^1 */
        }
        else
        {
            seq_t pow = pow_seq((seq_t)p, exp + 1); /* exp==1 */
            uint64_t mod5 = p % 5U;
            if (mod5 == 1U || mod5 == 4U)
            {
                term = (pow - 1) / ((seq_t)p - 1);
            }
            else    /* mod5 == 2 or 3 */
            {
                seq_t numerator = pow - 1;  /* exp odd -> (-1)^1 = -1 */
                term = numerator / ((seq_t)p + 1);
            }
        }
        result *= term;
    }

    return result;
}
