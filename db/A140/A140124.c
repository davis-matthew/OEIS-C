/*
 * A140124  a(n) = degree in N of the number of orbits under S_N of the set of
 *          n-tuples of partitions of {1,...,N} into n subsets.
 * https://oeis.org/A140124
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n^n - 1 - n*(n-1)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 16, u128 n <= 18,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* The sequence is defined only for n >= 2, but we guard
       against smaller indices for safety. */
    if (n < 2) return 0;

    /* Compute n^n using the provided big_pow helper. */
    seq_t power = big_pow((seq_t)n, (seq_t)n);

    /* Compute the subtraction term n*(n-1). */
    seq_t sub = (seq_t)n * ((seq_t)n - 1);

    /* Result: n^n - 1 - n*(n-1). */
    return power - 1 - sub;
}
