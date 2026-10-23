/*
 * A103198  Number of compositions of n into a square number of parts.
 * https://oeis.org/A103198
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Sum_{k>=0} binomial(n‑1, k^2‑1)
 * Range:     u8 n <= 10, u16 n <= 16, u32 n <= 31, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The case n=0 is defined to be 1.                     */
#include "../utils/binom.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;

    seq_t sum = 0;
    for (idx_t k = 1; ; ++k)
    {
        idx_t k2 = k * k;
        if (k2 > n)
            break;
        sum += binom(n - 1, k2 - 1);
    }
    return sum;
}
