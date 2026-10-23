/*
 * A100308  Modulo 2 binomial transform of 5^n.
 * https://oeis.org/A100308
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Sum_{k=0..n} (binomial(n, k) mod 2) * 5^k
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (binomial(n, k) mod 2 = 1 iff k is a submask of n) */
#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t sum = 0;

    /* iterate over all submasks k of n */
    for (idx_t k = n; ; k = (k - 1) & n)
    {
        /* compute 5^k by binary exponentiation */
        seq_t pow = 1;
        seq_t base = (seq_t)5;
        idx_t exp = k;
        while (exp)
        {
            if (exp & 1u)
                pow *= base;
            base *= base;
            exp >>= 1u;
        }

        sum += pow;

        if (k == 0)
            break;
    }

    return sum;
}
