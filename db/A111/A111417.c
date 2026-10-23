/*
 * A111417  a(n) = A034869(n) - A008311(n).
 * https://oeis.org/A111417
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = A001700(m) if n = (m+1)*(m+2), otherwise a(n)=0. Here
 *            A001700(m) = binomial(2*m+1, m)
 * Range:     u8 n <= 41, u16 n <= 91, u32 n <= 91, u64 n <= 91, u128 n <= 91,
 *            big any n
 * Helpers:   binom_small, int_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/int_sqrt.h"
#include "../utils/binom_small.h"

seq_t a(idx_t n)
{
    /* Compute s = floor(sqrt(1 + 4n)) */
    idx_t t = 4 * n;
    t += 1;
    idx_t s = int_sqrt(t);
    if (s * s != t)          /* not a perfect square */
        return 0;

    if (s < 3)               /* m would be negative */
        return 0;

    if ((s - 3) & 1)         /* (s - 3) not even, no integer m */
        return 0;

    idx_t m = (s - 3) / 2;
    return binom_small(2 * m + 1, m);
}
