/*
 * A077120  n^3 times nearest integer square to n^3.
 * https://oeis.org/A077120
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^3 * nearest integer square to n^3 (i.e., a(n) =
 *            A000578(n) * A077118(n), with A077118(n) = round(sqrt(n^3))^2 )
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 25, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;          /* n as seq_t */
    seq_t x  = nn * nn * nn;      /* n^3 */

    /* find nearest integer sqrt to x */
    seq_t s = floor_sqrt(x);          /* floor sqrt */
    seq_t diff_low  = x - s * s;      /* distance to s^2   */
    seq_t sp1 = s + 1;
    seq_t diff_high = sp1 * sp1 - x;  /* distance to (s+1)^2 */

    seq_t k = (diff_low < diff_high) ? s : sp1;  /* chosen nearest sqrt */

    seq_t k2 = k * k;            /* nearest integer square */
    return x * k2;               /* n^3 * nearest square */
}
