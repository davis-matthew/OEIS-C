/*
 * A075415  Squares of A002280 or numbers (666...6)^2.
 * https://oeis.org/A075415
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (4/9) * (10^(2n) – 2*10^n + 1) (square of the repunit of
 *            sixes)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 9, u128 n <= 18,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    /* 10^n  */
    seq_t ten_n = pow10(n);

    /* 10^(2n) = (10^n)^2  */
    seq_t ten_2n = ten_n * ten_n;

    /* numerator = 10^(2n) – 2*10^n + 1  */
    seq_t numerator = ten_2n - (seq_t)2 * ten_n + 1;

    /* a(n) = 4 * numerator / 9  */
    seq_t result = numerator * (seq_t)4 / (seq_t)9;

    return result;
}
