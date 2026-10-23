/*
 * A023974  First bit in fractional part of binary expansion of 7th root of n.
 * https://oeis.org/A023974
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 1 if (k-1/2)^7 < n < k^7 for some integer k, else 0
 * Range:     u8 n <= 17, u16 n <= 7814, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static seq_t power7(seq_t x)
{
    seq_t r = x;
    for (int i = 1; i < 7; ++i)
        r *= x;
    return r;
}

/* integer floor of the 7th root: largest t with t^7 <= x */
static seq_t floor_root7(seq_t x)
{
    if (x < 2)   /* 0 or 1 */
        return x;

    seq_t lo = 0;
    seq_t hi = 1;

    /* find hi such that hi^7 > x */
    while (true) {
        seq_t hi7 = power7(hi);
        if (hi7 > x)
            break;
        hi <<= 1;          /* hi = hi * 2 */
    }

    /* binary search in [lo, hi) */
    while (lo + 1 < hi) {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid7 = power7(mid);
        if (mid7 <= x)
            lo = mid;
        else
            hi = mid;
    }
    return lo;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* sequence is defined from n = 1 */

    seq_t seq_n = (seq_t)n;

    /* r = floor(n^(1/7)) */
    seq_t r = floor_root7(seq_n);
    seq_t r7 = power7(r);

    /* if n is a perfect 7th power: a(n) = 0 */
    if (r7 == seq_n)
        return 0;

    /* k = r + 1 = ceil(n^(1/7)) */
    seq_t k = r + 1;

    /* (k-0.5)^7 < n  <=>  (2k-1)^7 < 128 * n  */
    seq_t two_k_minus1 = k * 2 - 1;
    seq_t lhs = power7(two_k_minus1);
    seq_t rhs = seq_n * (seq_t)128;

    return (lhs < rhs) ? 1 : 0;
}
