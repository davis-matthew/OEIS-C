/*
 * A103198  Number of compositions of n into a square number of parts.
 * https://oeis.org/A103198
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Sum_{k>=0} binomial(n‑1, k^2‑1)
 * Range:     u8 n <= 11, u16 n <= 19, u32 n <= 36, u64 n <= 68, u128 n <= 132,
 *            big any n
 * Validated: standard, pass, 53 terms through n = 3281 (2026-11-01)
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The case n=0 is defined to be 1.                     */
#include <sys/types.h>
#include "../utils/binomial.h"

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
        sum += binomial(n - 1, k2 - 1);
    }
    return sum;
}
