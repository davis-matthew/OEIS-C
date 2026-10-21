/*
 * A084847  a(n) = 2*3^n+2^(2n-1)*(n-2).
 * https://oeis.org/A084847
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2*3^n + 2^(2n-1)*(n-2)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 14, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   pow3, pow4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow3.h"
#include "../utils/pow4.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)4;

    /* 3^n */
    seq_t threes = pow3(n);

    /* 4^n * (n-2) / 2 */
    seq_t temp = pow4(n);            /* 4^n  */
    temp *= (seq_t)(n - 2);          /* 4^n*(n-2)  */
    temp /= 2;                       /* divide by 2 (exact) */

    /* 2*3^n + temp */
    return threes * 2 + temp;
}
