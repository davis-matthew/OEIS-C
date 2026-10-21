/*
 * A088921  The number of 321- and 2143-avoiding permutations of length n.
 * https://oeis.org/A088921
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2^(n+1) - binomial(n+1,3) - 2*n - 1
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* sequence is defined for n ≥ 1 */

    /* 2^(n+1) */
    seq_t pow2_term = (seq_t)1;
    for (idx_t i = 0; i <= n; ++i)
        pow2_term <<= 1;          /* shift by 1 each iteration */

    /* binomial(n+1,3) = (n+1)*n*(n-1)/6 */
    seq_t binom = (seq_t)(n + 1);
    binom = binom * (seq_t)n / 2;
    binom = binom * (seq_t)(n - 1) / 3;

    /* linear subtraction term 2*n + 1 */
    seq_t sub = (seq_t)(2 * n + 1);

    return pow2_term - binom - sub;
}
