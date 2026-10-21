/*
 * A209591  Number of n X n 0..6 arrays with every element equal to a diagonal
 *          or antidiagonal reflection.
 * https://oeis.org/A209591
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 7 ^ (2*n-(n modulo 2)) * 91 ^ ((n*n-2*n+(n modulo 2))/4)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 5, u128 n <= 8,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n) {
    /* n >= 1 by OEIS offset; for n = 0 the formula is not defined */
    idx_t n_mod2 = n & 1U;                /* n modulo 2   */
    idx_t exp7_idx  = 2*n - n_mod2;       /* 2*n - (n%2)   */
    idx_t numer91 = n*n - 2*n + n_mod2;   /* n*n - 2*n + (n%2) */
    idx_t exp91_idx = numer91 / 4;        /* ((n*n-2*n+(n%2))/4) */

    seq_t pow7  = big_pow((seq_t)7,  (seq_t)exp7_idx);
    seq_t pow91 = big_pow((seq_t)91, (seq_t)exp91_idx);

    return pow7 * pow91;
}
