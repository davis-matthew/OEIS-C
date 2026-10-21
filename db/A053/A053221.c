/*
 * A053221  Row sums of triangle A053218.
 * https://oeis.org/A053221
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+2)*2^(n-1) - n - 1, n >= 1
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 28, u64 n <= 59, u128 n <= 122,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute 2^e as a seq_t using binary exponentiation. */
static seq_t pow2_exp(idx_t e)
{
    seq_t result = (seq_t)1;
    seq_t base   = (seq_t)2;
    while (e)
    {
        if (e & 1ULL)
            result *= base;
        base *= base;
        e >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* n is guaranteed to be >= 1, so n-1 is safe */
    seq_t nn = (seq_t)n;
    seq_t p  = pow2_exp(n - 1);          /* 2^(n-1)   */
    seq_t res = p * (nn + (seq_t)2);     /* (n+2)*2^(n-1) */
    res -= (nn + (seq_t)1);              /* subtract (n+1) */
    return res;
}
