/*
 * A066141  a(n) = n^(n-1) + n + 1.
 * https://oeis.org/A066141
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n^(n-1) + n + 1
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>


/* Compute base^exp in seq_t using binary exponentiation. */
static seq_t pow_seq_t(seq_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    while (exp != 0)
    {
        if (exp & 1ULL)
            result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* n is guaranteed to be ≥ 1 by the problem statement. */
    seq_t base = (seq_t)n;
    seq_t pow  = pow_seq_t(base, n - 1);
    return pow + base + (seq_t)1;
}
