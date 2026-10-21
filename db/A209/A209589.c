/*
 * A209589  Number of n X n 0..4 arrays with every element equal to a diagonal
 *          or antidiagonal reflection.
 * https://oeis.org/A209589
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5 ^ (2*n-(n modulo 2)) * 45 ^ ((n*n-2*n+(n modulo 2))/4)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 6, u128 n <= 9,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* n mod 2 */
    idx_t mod2 = n & 1ULL;

    /* first power: 5^(2*n - mod2) */
    seq_t pow5 = big_pow((seq_t)5,
                         (seq_t)(2ULL * n - mod2));

    /* second power: 45^((n*n - 2*n + mod2)/4) */
    idx_t temp = n * n - 2ULL * n + mod2;
    seq_t pow45 = big_pow((seq_t)45,
                          (seq_t)(temp / 4ULL));

    return pow5 * pow45;
}
