/*
 * A032519  Sum of the integer part of 11/3-th roots of integers less than n.
 * https://oeis.org/A032519
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=1}^{n} ⌊k^(3/11)⌋ (sum of the integer parts of the
 *            11/3‑th roots of 1…n)
 * Range:     u8 n <= 7, u16 n <= 50, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>   /* for bool, not really used but satisfies the style guide */

static seq_t pow_limit(seq_t base, int exp, seq_t limit)
{
    seq_t result = 1;
    for (int i = 0; i < exp; ++i)
    {
        /*  result * base  would exceed  limit  ->  stop */
        if (base != 0 && result > limit / base)
            return limit + 1;
        result *= base;
    }
    return result;
}

static seq_t integer_nth_root(seq_t value, int n)
{
    seq_t low = 0;
    seq_t high = 1;
    /* find an upper bound */
    while (pow_limit(high, n, value) <= value)
        high <<= 1;                /* high *= 2 */
    /* binary search in [low, high) */
    while (high - low > 1)
    {
        seq_t mid = low + (high - low) / 2;
        if (pow_limit(mid, n, value) <= value)
            low = mid;
        else
            high = mid;
    }
    return low;
}

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t k = 1; k <= n; ++k)
    {
        /* compute k^3 (fits in seq_t for the ranges used) */
        seq_t k3 = (seq_t)k * (seq_t)k * (seq_t)k;
        /* floor of k^(3/11) == floor of the 11th root of k^3 */
        seq_t r = integer_nth_root(k3, 11);
        sum += r;
    }
    return sum;
}
