/*
 * A009969  Powers of 25.
 * https://oeis.org/A009969
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 25^n
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 27,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    // Handle the base case explicitly
    if (n == 0)
        return (seq_t)1;

    /* Compute 25^n using binary exponentiation.
       big_pow performs at most O(log n) multiplications on seq_t.
       The exp argument is cast to seq_t to match the function signature. */
    return big_pow((seq_t)25, (seq_t)n);
}
