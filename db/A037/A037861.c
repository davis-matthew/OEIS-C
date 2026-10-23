/*
 * A037861  (Number of 0's) - (number of 1's) in the base-2 representation of
 *          n.
 * https://oeis.org/A037861
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, sign, look, easy
 * Formula:   a(n) = (#0 bits) minus (#1 bits) in binary representation of n,
 *            with a(0)=1 (binary "0" contains one 0 and no 1's)
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    /* Count of 1's in n. */
    unsigned int ones = __builtin_popcountll((unsigned long long)n);
    /* Number of bits needed to represent n (most‑significant bit index + 1). */
    unsigned int bits = 64 - __builtin_clzll((unsigned long long)n);

    /* a(n) = zeros - ones = bits - 2*ones.  Cast to seq_t before subtraction
       to avoid unsigned underflow. */
    return (seq_t)((seq_t)bits - (seq_t)(2 * ones));
}
