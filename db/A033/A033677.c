/*
 * A033677  Smallest divisor of n >= sqrt(n).
 * https://oeis.org/A033677
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   not stated
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) is the smallest divisor of n that is >= sqrt(n).
 * This is equivalent to finding the largest divisor d of n that is <= sqrt(n),
 * then returning n/d.
 */
#include <stdint.h>

static idx_t int_sqrt(idx_t n)
{
    idx_t lo = 0, hi = n;
    while (lo < hi) {
        idx_t mid = (lo + hi + 1) >> 1;
        // avoid overflow: use 128-bit intermediate if available
#if defined(__SIZEOF_INT128__)
        unsigned __int128 prod = (unsigned __int128)mid * mid;
        if (prod <= n)
            lo = mid;
        else
            hi = mid - 1;
#else
        // fallback for compilers without 128-bit: compare via division
        if (mid != 0 && n / mid < mid)
            hi = mid - 1;
        else
            lo = mid;
#endif
    }
    return lo;
}

seq_t a(idx_t n)
{
    if (n == 0) /* undefined, but return 0 for safety */
        return (seq_t)0;

    idx_t d = int_sqrt(n);
    while (d > 0) {
        if (n % d == 0)
            break;
        d--;
    }
    /* d will always be >=1, so division is safe */
    return (seq_t)n / (seq_t)d;
}
