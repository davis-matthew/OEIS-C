/*
 * A080763  Exchange 1's and 2's in the eta-sequence A006337.
 * https://oeis.org/A080763
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3 - floor((n+1)*sqrt(2)) + floor(n*sqrt(2))
 * Range:     u8 n <= 10, u16 n <= 660, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*          a(n) = 3 + floor(n*sqrt(2)) - floor((n+1)*sqrt(2)) */
/* The value is always 1 or 2 for n ≥ 1. */
/* Implementation uses an integer square‑root to compute the floors
 * of n*sqrt(2).  No floating point or <math.h> functions are used. */
/* Since seq_t may be an arbitrary precision integer type, we rely on
 * integer division to avoid overflow in the square‑root routine. */

#include <stdint.h>

/* integer square root: floor(sqrt(x)) */
static seq_t
isqrt(seq_t x)
{
    seq_t lo = 0;
    seq_t hi = x;
    while (lo < hi) {
        seq_t mid = (lo + hi + 1) / 2;
        if (mid <= x / mid)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

/* A080763: Exchange 1's and 2's in the eta‑sequence A006337. */
seq_t
a(idx_t n)
{
    /* The sequence is defined for n ≥ 1.  For n = 0 the function
     * returns 0 (unused, but prevents undefined behaviour). */
    if (n == 0)
        return 0;

    seq_t nn = (seq_t)n;
    seq_t r1 = isqrt(nn * nn * 2);
    seq_t np1 = nn + 1;
    seq_t r2 = isqrt(np1 * np1 * 2);

    /* 3 + r1 - r2 is always 1 or 2.  All arithmetic is performed
     * in unsigned seq_t, so the result is correct. */
    return 3 + r1 - r2;
}
