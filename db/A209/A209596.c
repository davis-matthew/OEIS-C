/*
 * A209596  Number of 5X5 0..n arrays with every element equal to a diagonal
 *          or antidiagonal reflection.
 * https://oeis.org/A209596
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n+1)^9 * ((n+1)*(2*n+1))^4
 * Range:     u8 none, u16 none, u32 n <= 2, u64 n <= 10, u128 n <= 50,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* For n < 1 the result is undefined; callers give n >= 1. */
    seq_t np1 = (seq_t)n + 1;            /* n + 1 */
    seq_t two_n = (seq_t)n * 2;          /* 2 * n */
    seq_t two_n_plus_one = two_n + 1;    /* 2 * n + 1 */
    seq_t inner = np1 * two_n_plus_one;  /* (n + 1) * (2 * n + 1) */

    seq_t pow1 = big_pow(np1, (seq_t)9);   /* (n + 1)^9 */
    seq_t pow2 = big_pow(inner, (seq_t)4); /* ((n + 1)(2n+1))^4 */

    return pow1 * pow2;
}
