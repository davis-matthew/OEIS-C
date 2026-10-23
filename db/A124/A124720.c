/*
 * A124720  Number of ternary Lyndon words of length n with exactly two 1's.
 * https://oeis.org/A124720
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(n-3)*(n-1) for n odd * a(n) = 2^(n-3)*(n-1) -
 *            2^(n/2-2) for n even * The offset is n = 3
 * Range:     u8 n <= 8, u16 n <= 15, u32 n <= 30, u64 n <= 61, u128 n <= 124,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n < 3) /* undefined behaviour for n < 3, return 0 */
        return (seq_t)0;

    /* compute 2^(n-3) */
    seq_t pow_n3 = pow2(n - 3);
    seq_t result = pow_n3 * (seq_t)(n - 1);

    if ((n & 1) == 0) {                /* even n */
        seq_t sub = pow2((n / 2) - 2);   /* n/2 - 2 is non‑negative for n>=4 */
        result -= sub;
    }

    return result;
}
