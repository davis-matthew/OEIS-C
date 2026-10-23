/*
 * A144615  a(n) = A000203(3n+2).
 * https://oeis.org/A144615
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = σ(3n+2) (sum of divisors of 3n+2)
 * Range:     u8 n <= 45, u16 n <= 8505, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Compute m = 3n+2, then the sum of its divisors. */
    idx_t m   = (idx_t)3 * n + (idx_t)2;
    seq_t res = (seq_t)1;
    idx_t tmp = m;

    /* Trial division factorisation.  The loop runs only up to sqrt(tmp). */
    for (idx_t p = (idx_t)2; p * p <= tmp; ++p)
    {
        if (tmp % p == (idx_t)0)
        {
            seq_t term   = (seq_t)1;
            seq_t prime_pow = (seq_t)1;
            while (tmp % p == (idx_t)0)
            {
                tmp      /= p;
                prime_pow  *= (seq_t)p;
                term      += prime_pow;
            }
            res *= term;
        }
    }

    /* Remaining prime factor larger than sqrt(original m). */
    if (tmp > (idx_t)1)
        res *= (seq_t)(1 + tmp);

    return res;
}
