/*
 * A102631  a(n) = n^2 / (squarefree kernel of n).
 * https://oeis.org/A102631
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = n^2 / (squarefree kernel of n)
 * Range:     u8 n <= 31, u16 n <= 511, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute base^exp where base and result are of type seq_t.
   exp is non‑negative and small (<= 127 for 64‑bit n). */
static seq_t pow_seq(seq_t base, int exp)
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
    if (n == 0)          /* offset is 1, but guard against 0 */
        return 0;

    seq_t result = 1;    /* accumulate the product of p^(2e-1) */
    idx_t m = n;         /* work on a copy of n */

    /* handle factor 2 separately (fast path for even numbers) */
    int e = 0;
    while ((m & 1) == 0)
    {
        m >>= 1;
        e++;
    }
    if (e > 0)
        result *= pow_seq((seq_t)2, 2 * e - 1);

    /* trial division for odd primes */
    for (idx_t p = 3; p <= m / p; p += 2)
    {
        if (m % p == 0)
        {
            e = 0;
            while (m % p == 0)
            {
                m /= p;
                e++;
            }
            result *= pow_seq((seq_t)p, 2 * e - 1);
        }
    }

    /* if a prime factor remains > 1 */
    if (m > 1)
        result *= pow_seq((seq_t)m, 1);   /* 2*1-1 = 1 */

    return result;
}
