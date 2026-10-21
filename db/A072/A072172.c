/*
 * A072172  a(n) = (2*n+1)*5^(2*n+1).
 * https://oeis.org/A072172
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*n+1)*5^(2*n+1)
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 5, u64 n <= 12, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

static seq_t pow5_uint64(uint64_t exp)
{
    seq_t result = (seq_t)1;
    seq_t base   = (seq_t)5;
    while (exp)
    {
        if (exp & 1ULL) result *= base;
        base *= base;
        exp >>= 1ULL;
    }
    return result;
}

seq_t a(idx_t n)
{
    seq_t coeff = ((seq_t)n) * 2 + 1;          /* (2*n+1) */
    uint64_t exp = 2ULL * n + 1ULL;            /* 2*n+1 */
    seq_t power = pow5_uint64(exp);            /* 5^(2*n+1) */
    return coeff * power;
}
