/*
 * A019545  Cubes whose digits are cubes.
 * https://oeis.org/A019545
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 0 for n=1; a(n) = (n%2==0 ? 1 : 8) * 10^(3 *
 *            floor((n-2)/2)) for n >= 2
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 15, u128 n <= 19,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n) {
    if (n == 1) return 0;
    idx_t k = (n - 2) / 2;                 /* floor((n-2)/2) */
    seq_t base = pow10(3 * k);             /* 10^(3*k) */
    return (n & 1) ? base * 8 : base;      /* odd => *8, even => *1 */
}
