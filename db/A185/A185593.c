/*
 * A185593  a(n) = floor(n^(3/2))*floor(3+n^(3/2))/2.
 * https://oeis.org/A185593
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(n^(3/2))*floor(3+n^(3/2))/2
 * Range:     u8 n <= 7, u16 n <= 50, u32 n <= 2046, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   floor_n3_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/floor_n3_sqrt.h"

seq_t a(idx_t n)
{
    /* sequence offset is 1, so n≥1 in practice */
    seq_t t = floor_n3_sqrt(n);          /* floor(n^(3/2)) */
    seq_t res = t * (t + 3) / 2;         /* floor(n^(3/2)) * floor(3+n^(3/2)) / 2 */
    return res;
}
