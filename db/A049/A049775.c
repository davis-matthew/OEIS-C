/*
 * A049775  a(n) is the sum of all integers from 2^(n-2)+1 to 2^(n-1).
 * https://oeis.org/A049775
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(n‑3) * (3*2^(n‑2) + 1) (n ≥ 2)
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n < 2)
        return (seq_t)0;          /* undefined for n < 2, return 0 */
    if (n == 2)
        return (seq_t)2;          /* base case */

    seq_t p2_n_minus_2 = pow2(n - 2);   /* 2^(n-2) */
    seq_t p2_n_minus_3 = pow2(n - 3);   /* 2^(n-3) */
    seq_t tmp = 3 * p2_n_minus_2 + 1;   /* 3*2^(n-2)+1 */
    return p2_n_minus_3 * tmp;          /* 2^(n-3)*(3*2^(n-2)+1) */
}
