/*
 * A206460  Number of 0..n arrays of length n+6 avoiding the consecutive
 *          pattern 0..n.
 * https://oeis.org/A206460
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{i=0..⌊(n+6)/(n+1)⌋} (-1)^i * (n+1)^((n+6)-(n+1)i) *
 *            binomial((n+6)-n i, i)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 22,
 *            big any n
 * Helpers:   binomial, pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_seq.h"
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t sum_even = 0;
    seq_t sum_odd  = 0;
    idx_t max_i = (n + 6) / (n + 1);            /* floor((n+6)/(n+1)) */
    seq_t base = (seq_t)(n + 1);

    for (idx_t i = 0; i <= max_i; ++i) {
        idx_t exp = (n + 6) - (n + 1) * i;      /* (n+6) - (n+1)*i */
        seq_t power = pow_seq(base, (int)exp);
        seq_t binom = binomial((n + 6) - n * i, i);
        seq_t term = power * binom;

        if ((i & 1) == 0)
            sum_even += term;
        else
            sum_odd += term;
    }

    return sum_even - sum_odd;
}
