/*
 * A216477  The sequence of the parts in the partition binary diagram
 *          represented as an array.
 * https://oeis.org/A216477
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   If n = k² or n = k² + k then a(n) = ⌈√(4n)⌉ – 1; otherwise a(n)
 *            = ⌊ (⌈√(4n)⌉²) / 4 ⌋ – n
 * Range:     u8 n <= 15, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/floor_sqrt.h"   /* provides seq_t floor_sqrt(seq_t) */

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* the sequence is defined for n ≥ 1 */

    /* compute ceil(sqrt(4n)) */
    seq_t four_n = (seq_t)4 * (seq_t)n;
    seq_t s      = floor_sqrt(four_n);            /* floor sqrt(4n)   */
    seq_t ceil_sqrt = s;
    if (s * s != four_n)                           /* not a perfect square */
        ceil_sqrt = s + 1;

    /* check special cases n = k² or n = k² + k */
    seq_t k   = floor_sqrt((seq_t)n);              /* floor sqrt(n)    */
    seq_t k2  = k * k;
    if ((seq_t)n == k2 || (seq_t)n == k2 + k)
        return ceil_sqrt - 1;

    /* otherwise the general formula */
    seq_t val = (ceil_sqrt * ceil_sqrt) / 4;       /* floor(ceil²/4)   */
    return val - (seq_t)n;
}
