/*
 * A055582  Sixth column of triangle A055252.
 * https://oeis.org/A055582
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (n^2 - 3*n + 8)*(2^(n+3) - 1)/2 - 9*(n + 3)
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 21, u64 n <= 50, u128 n <= 112,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Compute (n^2 - 3*n + 8) */
    seq_t n_seq = (seq_t)n;
    seq_t part1 = n_seq * n_seq - (seq_t)3 * n_seq + (seq_t)8;

    /* M = (2^(n+3) - 1)  (an odd number) */
    seq_t M = pow2(n + 3) - (seq_t)1;

    /* Compute part1 * M divided by 2 without overflow.
       For an integer x: (x * y) / 2 == ((x >> 1) * y) + ((x & 1) * (y >> 1))   */
    seq_t half_product = (part1 >> 1) * M;
    if (part1 & 1)
        half_product += (M >> 1);

    /* Subtract 9*(n + 3) */
    seq_t subtract = (seq_t)9 * (n_seq + (seq_t)3);
    return half_product - subtract;
}
