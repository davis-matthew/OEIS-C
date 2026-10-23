/*
 * A028392  a(n) = n + floor(sqrt(n)).
 * https://oeis.org/A028392
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n + floor(sqrt(n))
 * Range:     u8 n <= 63, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* Compute integer floor of sqrt(n).  floor_sqrt works on seq_t,
       so cast n to seq_t first. */
    seq_t s = floor_sqrt((seq_t)n);
    /* Return n + floor(sqrt(n)).  Cast n to seq_t for the addition. */
    return (seq_t)n + s;
}
