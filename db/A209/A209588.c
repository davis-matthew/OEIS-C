/*
 * A209588  Number of n X n 0..3 arrays with every element equal to a diagonal
 *          or antidiagonal reflection.
 * https://oeis.org/A209588
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4 ^ (2*n-(n modulo 2)) * 28 ^ ((n*n-2*n+(n modulo 2))/4)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 4, u64 n <= 6, u128 n <= 9,
 *            big any n
 * Helpers:   pow_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_int.h"

seq_t a(idx_t n)
{
    /* n modulo 2 (0 or 1) */
    idx_t mod = n & 1ULL;

    /* exponent of 4 : 2*n - (n mod 2) */
    idx_t exp1 = n + n - mod;          /* using n+n to avoid overflow of 2*n */

    /* exponent of 28 : (n*n - 2*n + (n mod 2))/4 */
    unsigned __int128 tmp = (unsigned __int128)n * n;  /* n*n in 128‑bit */
    idx_t exp2 = (tmp - 2*n + mod) / 4;

    /* compute the powers using the provided helper */
    seq_t pow4  = pow_int((seq_t)4,  exp1);
    seq_t pow28 = pow_int((seq_t)28, exp2);

    /* final product */
    return pow4 * pow28;
}
