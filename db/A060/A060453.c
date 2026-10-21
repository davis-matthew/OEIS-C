/*
 * A060453  Dot product of the squares and the quarter-squares: a(n) =
 *          sum(i=1..n, i^2 * floor(i^2/4)).
 * https://oeis.org/A060453
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ{i=1..n} i^2 * floor(i^2 / 4) (offset 1)
 * Range:     u8 n <= 5, u16 n <= 16, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    seq_t sum = 0;
    for (idx_t i = 1; i <= n; ++i) {
        seq_t i2 = (seq_t)i * (seq_t)i;          /* i^2 */
        seq_t q  = i2 / 4;                       /* floor(i^2 / 4) */
        sum += i2 * q;                           /* i^2 * floor(i^2/4) */
    }
    return sum;
}
