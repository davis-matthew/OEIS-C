/*
 * A172473  a(n) = floor(sqrt(2*n^5)).
 * https://oeis.org/A172473
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(sqrt(2*n^5))
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 45, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    seq_t sn  = (seq_t)n;
    seq_t n2  = sn * sn;            /* n^2 */
    seq_t n3  = n2 * sn;            /* n^3 */
    seq_t n5  = n3 * n2;            /* n^5 = n^3 * n^2 */
    seq_t val = n5 * ((seq_t)2);    /* 2*n^5 */
    return floor_sqrt(val);         /* floor of sqrt(2*n^5) */
}
