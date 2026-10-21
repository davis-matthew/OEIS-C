/*
 * A057010  Number of conjugacy classes of subgroups of index 4 in free group
 *          of rank n.
 * https://oeis.org/A057010
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 24^(n-1) + 8^(n-1) – 6^(n-1) (n ≥ 1)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 14, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Computes base^exp using exponentiation by squaring. */
static inline seq_t pow_seq_t(seq_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    while (exp)
    {
        if (exp & (idx_t)1) result *= base;
        base *= base;
        exp >>= (idx_t)1;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* n ≥ 1 (offset is 1).  For n = 1, exponent is 0. */
    idx_t e = n - (idx_t)1;          /* (seq_t)n – 1 is safe because n ≥ 1 */
    seq_t p24 = pow_seq_t((seq_t)24, e);
    seq_t p8  = pow_seq_t((seq_t)8,  e);
    seq_t p6  = pow_seq_t((seq_t)6,  e);
    return p24 + p8 - p6;
}
