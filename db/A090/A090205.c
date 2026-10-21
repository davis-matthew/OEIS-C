/*
 * A090205  a(n) = n^n * (n+1)^(n+1).
 * https://oeis.org/A090205
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n^n * (n+1)^(n+1)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 9, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t pow_seq_t(seq_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    while (exp)
    {
        if (exp & (idx_t)1) result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    seq_t part1 = pow_seq_t((seq_t)n, n);
    seq_t part2 = pow_seq_t((seq_t)(n + (idx_t)1), (n + (idx_t)1));
    return part1 * part2;
}
