/*
 * A083667  Number of antisymmetric binary relations on a set of n labeled
 *          points.
 * https://oeis.org/A083667
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2^n * 3^(n*(n-1)/2)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 8, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Helper to compute base^exp for integer exponents.
   Works for any seq_t that supports operators * and =. */
static seq_t pow_seq(seq_t base, unsigned long long exp)
{
    seq_t res = 1;
    while (exp)
    {
        if (exp & 1ULL)
            res *= base;
        base *= base;
        exp >>= 1;
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;          /* a(0) = 1 */

    /* 2^n */
    seq_t pow2 = pow_seq((seq_t)2, (unsigned long long)n);

    /* exponent for 3: n*(n-1)/2 */
    unsigned long long t = (unsigned long long)n * (n - 1ULL) / 2ULL;
    seq_t pow3 = pow_seq((seq_t)3, t);

    return pow2 * pow3;
}
