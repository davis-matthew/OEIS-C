/*
 * A064478  If n = Product p(k)^e(k) then a(n) = Product (p(k)+1)^e(k), a(0) =
 *          1, a(1)=2.
 * https://oeis.org/A064478
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = ∏_{p^e∥n} (p+1)^e with a(0)=1, a(1)=2
 * Range:     u8 n <= 47, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* multiply base^exp; exp is small enough that repeated squaring works */
static seq_t pow_seq_t_local(seq_t base, idx_t exp)
{
    seq_t res = 1;
    while (exp > 0)
    {
        if (exp & 1)
            res *= base;
        base *= base;
        exp >>= 1;
    }
    return res;
}

seq_t a(idx_t n)
{
    /* handle the special cases */
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)2;

    seq_t result = 1;
    idx_t m = n;

    /* factor out the prime 2 first */
    if ((m & 1) == 0)
    {
        idx_t e = 0;
        while ((m & 1) == 0)
        {
            m >>= 1;
            ++e;
        }
        result *= pow_seq_t_local((seq_t)(2 + 1), e);
    }

    /* factor odd primes */
    for (idx_t i = 3; i <= m / i; i += 2)
    {
        if (m % i == 0)
        {
            idx_t e = 0;
            while (m % i == 0)
            {
                m /= i;
                ++e;
            }
            result *= pow_seq_t_local((seq_t)(i + 1), e);
        }
    }

    /* if something left, it is a prime > sqrt(original n) */
    if (m > 1)
        result *= pow_seq_t_local((seq_t)(m + 1), 1);

    return result;
}
