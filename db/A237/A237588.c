/*
 * A237588  a(n) = sigma(n) - 2n + 1.
 * https://oeis.org/A237588
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = sigma(n) - 2*n + 1
 * Range:     i8 n <= 64, i16 n <= 64, i32 n <= 64, i64 n <= 64, i128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Compute sigma(n), the sum of all positive divisors of n */
    seq_t sigma = 0;
    uint64_t nn = n;

    for (uint64_t i = 1; i * i <= nn; ++i)
    {
        if (nn % i == 0)
        {
            sigma += (seq_t)i;
            uint64_t j = nn / i;
            if (j != i)
                sigma += (seq_t)j;
        }
    }

    /* a(n) = sigma(n) - 2*n + 1 */
    seq_t res = sigma - ((seq_t)2 * (seq_t)n) + 1;
    return res;
}
