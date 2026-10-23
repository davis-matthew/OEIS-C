/*
 * A183222  Complement of the 10-gonal numbers.
 * https://oeis.org/A183222
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + floor((sqrt(n)+1)/2)
 * Range:     u8 n <= 63, u16 n <= 97, u32 n <= 97, u64 n <= 97, u128 n <= 97,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* n is 1‑based (offset 1) */
    seq_t root = floor_sqrt((seq_t)n);   /* floor(sqrt(n)) */
    seq_t add  = (root + 1) >> 1;        /* floor((sqrt(n)+1)/2) */
    return (seq_t)n + add;
}
