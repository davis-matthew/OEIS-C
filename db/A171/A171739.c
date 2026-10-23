/*
 * A171739  a(n) = 2^(n*(n-1)/2)*3^(n*(n+1)/2).
 * https://oeis.org/A171739
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(n*(n-1)/2)*3^(n*(n+1)/2)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 6, u128 n <= 9,
 *            big any n
 * Helpers:   pow2, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/pow3.h"

seq_t a(idx_t n)
{
    /* avoid unsigned underflow for n==0 */
    idx_t exp1 = n == 0 ? 0 : n * (n - 1) / 2;
    idx_t exp2 = n * (n + 1) / 2;

    seq_t part2 = pow2(exp1);
    seq_t part3 = pow3(exp2);
    return part2 * part3;
}
