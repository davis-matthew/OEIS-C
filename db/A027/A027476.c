/*
 * A027476  Third column of A027467.
 * https://oeis.org/A027476
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 15^(n-3) * binomial(n-1, 2)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 17, u128 n <= 33,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* For n < 3 the behaviour is undefined; return 0 for safety. */
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n < 3)
        return 0;   /* offset: a(3) is the first term */
    /* Compute the binomial factor (n-1)(n-2)/2 */
    seq_t t1 = (seq_t)(n - 1);
    seq_t t2 = (seq_t)(n - 2);
    seq_t bin = t1 * t2 / 2;
    /* Compute 15^(n-3) */
    seq_t pow_val = big_pow((seq_t)15, (seq_t)(n - 3));
    /* Final result */
    return pow_val * bin;
}
