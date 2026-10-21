/*
 * A209586  Number of n X n 0..1 arrays with every element equal to a diagonal
 *          or antidiagonal reflection.
 * https://oeis.org/A209586
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2 ^ (2*n-(n%2)) * 6 ^ ((n*n-2*n+(n%2))/4)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 9, u128 n <= 13,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* n >= 1 */
    idx_t n_mod2 = n & 1U;                 /* n % 2 */
    idx_t e2 = 2 * n - n_mod2;            /* exponent for 2 */
    idx_t e6 = (n * n - 2 * n + n_mod2) / 4; /* exponent for 6 */

    seq_t pow2 = big_pow((seq_t)2, (seq_t)e2);
    seq_t pow6 = big_pow((seq_t)6, (seq_t)e6);

    return pow2 * pow6;
}
