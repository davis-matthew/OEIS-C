/*
 * A017571  a(n) = (12n+4)^3.
 * https://oeis.org/A017571
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (12*n + 4)^3
 * Range:     u8 n <= 0, u16 n <= 3, u32 n <= 135, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Convert n to a larger integer type to avoid overflow for the intermediate
       calculations.  unsigned long long is guaranteed to be at least 64 bits.  */
    unsigned long long base = 12ULL * (unsigned long long)n + 4ULL;
    unsigned long long cube = base * base * base;
    return (seq_t)cube;
}
