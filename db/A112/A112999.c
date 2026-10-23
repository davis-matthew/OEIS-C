/*
 * A112999  Partial sums of A036740.
 * https://oeis.org/A112999
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Sum_{k=1..n} (k!)^k
 * Range:     u8 n <= 3, u16 n <= 3, u32 n <= 4, u64 n <= 6, u128 n <= 8,
 *            big any n
 * Helpers:   pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_seq.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    seq_t fact = 1;

    for (idx_t k = 1; k <= n; ++k)
    {
        fact *= (seq_t)k;          // fact = k!
        sum += pow_seq(fact, k);   // add (k!)^k
    }

    return sum;
}
