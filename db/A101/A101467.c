/*
 * A101467  Number of distinct n-term ratios x_1 : x_2 : ... : x_n where each
 *          x_i is in the range [1-10].
 * https://oeis.org/A101467
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1) = 10; for n>1, a(n) = 10^n - 5^n - 3^n - 2^n + 1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* compute base^exp in seq_t; uses repeated multiplication (O(exp)) */
static seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t res = (seq_t)1;
    for (idx_t i = 0; i < exp; ++i)
        res *= base;
    return res;
}

seq_t a(idx_t n)
{
    if (n < 1) /* undefined for offset < 1, but keep defined */
        return (seq_t)0;

    if (n == 1)
        return (seq_t)10;

    /* compute 10^n, 5^n, 3^n, 2^n */
    seq_t p10 = pow_seq((seq_t)10, n);
    seq_t p5  = pow_seq((seq_t)5, n);
    seq_t p3  = pow_seq((seq_t)3, n);
    seq_t p2  = pow_seq((seq_t)2, n);

    /* a(n) = 10^n - 5^n - 3^n - 2^n + 1 */
    seq_t result = p10 - p5 - p3 - p2 + (seq_t)1;
    return result;
}
