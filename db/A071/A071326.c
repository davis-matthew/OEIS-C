/*
 * A071326  Sum of squares > 1 dividing n.
 * https://oeis.org/A071326
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (∏_{p^e‖n} (∑_{k=0}^{⌊e/2⌋} p^{2k})) - 1
 * Range:     u8 n <= 224, u16 n <= 9900, u32 n <= 9900, u64 n <= 9900,
 *            u128 n <= 9900, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*   where the product runs over the prime‐power factors of n.      */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n <= 1)            /* offset 1: a(1) = 0 */
        return 0;

    seq_t prod = 1;
    idx_t temp = n;

    for (idx_t p = 2; p * p <= temp; ++p)
    {
        if (temp % p == 0)
        {
            /* count exponent of prime p in n */
            idx_t exp = 0;
            while (temp % p == 0)
            {
                temp /= p;
                exp++;
            }

            /* compute ∑_{k=0}^{⌊exp/2⌋} p^{2k} */
            seq_t sum = 1;          /* term for k = 0 (p^0) */
            seq_t pow = 1;          /* current value of p^{2k} */
            seq_t p2 = (seq_t)p * (seq_t)p;

            for (idx_t k = 0; k < exp / 2; ++k)
            {
                pow *= p2;              /* p^{2(k+1)} */
                sum += pow;             /* add to the sum for this prime */
            }

            prod *= sum;
        }
    }

    /* If temp > 1 here, it is a prime factor with exponent 1
       whose contribution to the product is 1 (since ⌊1/2⌋ = 0). */

    prod -= 1;                 /* exclude the divisor 1 */
    return prod;
}
