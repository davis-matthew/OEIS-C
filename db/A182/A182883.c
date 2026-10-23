/*
 * A182883  Number of weighted lattice paths of weight n having no (1,0)-steps
 *          of weight 1.
 * https://oeis.org/A182883
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k = 0 .. floor(n/2)} binomial(n-k, k) * binomial(k,
 *            n-2*k)
 * Range:     u8 n <= 11, u16 n <= 20, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t maxk = n / 2;
    for (idx_t k = 0; k <= maxk; ++k) {
        idx_t m = n - 2 * k;
        seq_t t1 = binom(n - k, k);
        seq_t t2 = binom(k, m);
        sum += t1 * t2;
    }
    return sum;
}
