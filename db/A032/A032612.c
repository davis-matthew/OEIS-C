/*
 * A032612  Concatenation of n and n+7.
 * https://oeis.org/A032612
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = concat(n, n+7) = n * 10^digits(n+7) + (n+7)
 * Range:     u8 n <= 2, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   digit_count, pow10, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/digit_count.h"
#include "../utils/pow10.h"
#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    /* Ensure offset 1: return 0 for n==0 (unused but safe) */
    if (n == 0)
        return (seq_t)0;

    /* Compute n+7 as uint64_t (fits in idx_t range) */
    uint64_t m = (uint64_t)n + 7;

    /* Number of decimal digits of m */
    unsigned d = digit_count(m);

    /* 10^d as seq_t */
    seq_t pow10_val = pow10((idx_t)d);

    /* n * 10^d as seq_t */
    seq_t prod = seq_mul((seq_t)n, pow10_val);

    /* Final result */
    return prod + (seq_t)m;
}
