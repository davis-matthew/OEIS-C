/*
 * A084869  Number of 2-multiantichains of an n-set.
 * https://oeis.org/A084869
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (4^n - 2*3^n + 3*2^n)/2
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   mul_small, pow2, pow3, pow4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/pow3.h"
#include "../utils/pow4.h"
#include "../utils/mul_small.h"

seq_t a(idx_t n)
{
    seq_t t4 = pow4(n);
    seq_t t3 = pow3(n);
    seq_t t2 = pow2(n);
    seq_t res = t4;
    res += mul_small(t2, 3);   /* add 3*2^n */
    res -= mul_small(t3, 2);   /* subtract 2*3^n */
    res /= 2;                  /* divide by 2 */
    return res;
}
