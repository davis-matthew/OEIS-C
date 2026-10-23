/*
 * A135203  For any integer n >= 1 the sequence gives the minimum power x for
 *          which n^x+(n-1)^x+(n-2)^x+...+1^x produces a perfect square.
 * https://oeis.org/A135203
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 31, u16 n <= 9999, u32 n <= 9999, u64 n <= 9999,
 *            u128 n <= 9999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Minimal exponent x such that ∑_{k=1}^n k^x is a perfect square.
   Testing x = 1, 2 and 3 suffices for all n ≥ 1. */

#include <stdbool.h>

/* Integer square root by binary search, works for any seq_t.
   Returns ⌊√v⌋. The algorithm avoids overflow by using division
   instead of squaring the mid value. */
static inline seq_t _floor_isqrt(seq_t v)
{
    if (v == 0)
        return 0;
    seq_t low = 1;
    seq_t high = v;
    seq_t ans = 0;

    while (low <= high) {
        seq_t mid = low + (high - low) / 2;
        /* Compare mid^2 with v using division: mid <= v / mid iff mid^2 <= v */
        seq_t div = v / mid;
        if (mid > div) {          /* mid^2 > v */
            high = mid - 1;
        } else {                  /* mid^2 ≤ v */
            ans = mid;
            low = mid + 1;
        }
    }
    return ans;
}

/* Test whether a seq_t value is a perfect square. */
static inline bool _is_square(seq_t x)
{
    seq_t r = _floor_isqrt(x);
    return r * r == x;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;

    /* Sum for exponent 1: n(n+1)/2 */
    seq_t sum1 = nn * (nn + 1) / 2;
    if (_is_square(sum1))
        return (seq_t)1;

    /* Sum for exponent 2: n(n+1)(2n+1)/6 */
    seq_t sum2 = nn * (nn + 1) * (2 * nn + 1) / 6;
    if (_is_square(sum2))
        return (seq_t)2;

    /* Sum for exponent 3 is always a square: (n(n+1)/2)^2 */
    return (seq_t)3;
}
