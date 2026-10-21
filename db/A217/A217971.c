/*
 * A217971  a(n) = 2^(2*n+1) * (2*n+1)*n^(2*n).
 * https://oeis.org/A217971
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^(2*n+1) * (2*n+1) * n^(2*n)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 7, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* fast binary exponentiation for arbitrary-size seq_t
   base^exp where exp fits in idx_t. */
static seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t result = 1;
    while (exp != 0)
    {
        if (exp & 1U) result *= base;
        base *= base;
        exp >>= 1U;
    }
    return result;
}

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;
    /* 2^(2*n+1) */
    seq_t term = pow_seq((seq_t)2, 2*n + 1);
    /* multiply by (2*n+1) */
    term *= (seq_t)(2*n + 1);
    /* multiply by n^(2*n) */
    term *= pow_seq(N, 2*n);

    return term;
}
