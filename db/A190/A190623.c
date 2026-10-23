/*
 * A190623  Mobius transform of A008457.
 * https://oeis.org/A190623
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = product_{p^e‖n} f(p,e) with f(2,1)=6, f(2,e)=8^e for e>1,
 *            and f(p,e)=p^(3e) for p>2
 * Range:     u8 n <= 6, u16 n <= 40, u32 n <= 1626, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* fast exponentiation for seq_t */
static seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    while (exp)
    {
        if (exp & (idx_t)1) result *= base;
        base *= base;
        exp >>= (idx_t)1;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n == (idx_t)1) return (seq_t)1;

    seq_t result = (seq_t)1;
    idx_t temp = n;

    for (idx_t p = (idx_t)2; p <= temp / p; ++p)
    {
        if (temp % p == (idx_t)0)
        {
            int exp = (int)0;
            while (temp % p == (idx_t)0)
            {
                temp /= p;
                exp++;
            }

            seq_t val;
            if (p == (idx_t)2)
            {
                if (exp == 1)
                    val = (seq_t)6;
                else
                    val = pow_seq((seq_t)8, (idx_t)exp);
            }
            else
            {
                val = pow_seq((seq_t)p, (idx_t)(3 * exp));
            }
            result *= val;
        }
    }

    /* Remaining prime factor, if any */
    if (temp > (idx_t)1)
    {
        seq_t val;
        if (temp == (idx_t)2)
            val = (seq_t)6;          /* only possible when n is 2 */
        else
            val = pow_seq((seq_t)temp, (idx_t)3);   /* exp = 1 */
        result *= val;
    }

    return result;
}
