/*
 * A240471  Integer part of (n * A000005(n) / A000203(n)).
 * https://oeis.org/A240471
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ⌊ n * d(n) / σ(n) ⌋ (d(n) = number of divisors, σ(n) =
 *            sum of divisors)
 * Range:     u8 n <= 35, u16 n <= 6203, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   divisor_count_u64, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/divisor_count_u64.h"
#include "../utils/sigma_of.h"

seq_t a(idx_t n)
{
    if (n == 0) /* offset is 1, a(0) is undefined; return 0 */ 
        return 0;
    /* d(n) is divisor_count_u64 returning seq_t, sigma(n) is sigma_of returning seq_t */
    seq_t d   = divisor_count_u64((uint64_t)n);
    seq_t sigma = sigma_of(n);
    seq_t prod = (seq_t)n * d;
    return prod / sigma;
}
