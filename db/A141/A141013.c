/*
 * A141013  E.g.f. Sum_{d|M} (exp(d*x)-1)/d, M=14.
 * https://oeis.org/A141013
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 17, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A141013
 * Formula: a(n) = 0                     , n = 0
 *          a(n) = 1 + 2^(n-1) + 7^(n-1) + 14^(n-1)  , n ≥ 1
 */

#include <stddef.h>  /* idx_t is provided by the build */

static seq_t pow_seq_u(seq_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    for (idx_t i = 0; i < exp; ++i)
        result *= base;
    return result;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    idx_t e = n - 1;
    seq_t sum = (seq_t)1;                     /* 1^(n-1) */
    sum += pow_seq_u((seq_t)2,  e);           /* 2^(n-1) */
    sum += pow_seq_u((seq_t)7,  e);           /* 7^(n-1) */
    sum += pow_seq_u((seq_t)14, e);           /*14^(n-1) */
    return sum;
}
