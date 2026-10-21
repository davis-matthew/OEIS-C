/*
 * A221022  Equals one maps: number of n X 4 binary arrays indicating the
 *          locations of corresponding elements equal to exactly one of their
 *          horizontal, vertical and antidiagonal neighbors in a random 0..3 n
 *          X 4 array.
 * https://oeis.org/A221022
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 16^n - 81*2^(2*n-3) (n > 3) For n = 1,2,3 a(1)=8,
 *            a(2)=168, a(3)=3474
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 32,
 *            big any n
 * Helpers:   mul_small, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/mul_small.h"

seq_t a(idx_t n)
{
    /* Handle small n explicitly */
    if (n == 0) return 0;
    if (n == 1) return 8;
    if (n == 2) return 168;
    if (n == 3) return 3474;

    /* 16^n = 2^(4*n) */
    seq_t pow4n = pow2(4 * n);

    /* 81*2^(2*n-3) */
    seq_t pow2_2n_3 = pow2(2 * n - 3);
    seq_t sub = mul_small(pow2_2n_3, 81);

    return pow4n - sub;
}
