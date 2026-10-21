/*
 * A209590  Number of n X n 0..5 arrays with every element equal to a diagonal
 *          or antidiagonal reflection.
 * https://oeis.org/A209590
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6 ^ (2*n-(n modulo 2)) * 66 ^ ((n*n-2*n+(n modulo 2))/4)
 * Range:     u8 n <= 1, u16 n <= 2, u32 none, u64 n <= 5, u128 n <= 8,
 *            big any n
 * Helpers:   pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_seq.h"

seq_t a(idx_t n)
{
    /* The sequence is defined only for n >= 1 (offset 1). */
    if (n == 0)
        return 0;

    /* Compute the two exponents. */
    idx_t mod2   = n & 1;                           /* n mod 2 */
    seq_t expo1  = (seq_t)(2 * n - mod2);           /* 2*n - (n mod 2) */
    seq_t expo2  = ((seq_t)n * (seq_t)n - 2 * (seq_t)n + mod2) / 4;

    /* Use pow_seq which performs exponentiation by squaring. */
    seq_t p1 = pow_seq((seq_t)6,  (int)expo1);
    seq_t p2 = pow_seq((seq_t)66, (int)expo2);

    return p1 * p2;
}
