/*
 * A140215  A140213(n)*A140214(n).
 * https://oeis.org/A140215
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   * a(n) = A140213(n) * A140214(n) * * Computation: * 1. Let m be
 *            n with all factors of 2 removed (odd part). * 2. Remove all
 *            factors of 3 from m. * 3. If m == 1 return 1. * 4. Factor m into
 *            primes greater than 3: * m = p1^e1 * p2^e2 * ... * pk^ek (k ≥ 1)
 *            * 5. If k == 1: * • if e1 == 1 return p1; *...
 * Range:     u8 n <= 34, u16 n <= 76, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;   /* undefined; safe fallback */

    /* Step 1: remove all factors of 2 */
    idx_t m = n;
    while ((m & 1) == 0) m >>= 1;

    /* Step 2: remove all factors of 3 */
    while (m % 3 == 0) m /= 3;

    if (m == 1) return (seq_t)1;   /* after removing 3s nothing remains */

    /* Step 4: factor m (only primes > 3 remain) */
    idx_t distinct = 0;            /* number of distinct primes */
    idx_t prod_primes = 1;         /* product of distinct primes */
    idx_t exponent = 0;            /* exponent of the last prime found */
    idx_t temp = m;

    for (idx_t p = 3; p <= temp / p; p += 2)   /* p runs over odd candidates */
    {
        if (temp % p == 0)                     /* prime factor found */
        {
            ++distinct;
            prod_primes *= p;
            exponent = 0;
            while (temp % p == 0)
            {
                temp /= p;
                ++exponent;
            }
        }
    }

    if (temp > 1)                             /* remaining prime factor */
    {
        ++distinct;
        prod_primes *= temp;
        exponent = 1;                         /* it appears only once */
    }

    /* Step 5: produce the result */
    if (distinct == 1)        /* single prime factor */
    {
        if (exponent == 1)
            return (seq_t)prod_primes;                  /* p^1 */
        else
            return seq_pow((seq_t)prod_primes, exponent + 1); /* p^(e+1) */
    }
    else                      /* at least two distinct primes */
    {
        return seq_pow((seq_t)prod_primes, 2);          /* (∏p)^2 */
    }
}
