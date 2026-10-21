/*
 * A231353  Least k>=0 such that n - k^2 is a triangular number, or -1 if no
 *          such k exists.
 * https://oeis.org/A231353
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = min{k >= 0 | n - k^2 is a triangular number} answer is -1
 *            if no such k exists, otherwise the smallest k. Triangular
 *            numbers: T_m = m(m+1)/2
 * Range:     i8 n <= 82, i16 n <= 82, i32 n <= 82, i64 n <= 82, i128 n <= 82,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Test whether 128-bit unsigned integer v is a perfect square.
   Only integer operations are used. */
static bool is_square(unsigned __int128 v)
{
    if (v < 2) return true;           /* 0 and 1 are squares */

    /* Upper bound for sqrt(v).  For n <= 2^64-1,
       v = 8*n + 1 < 2^67, so sqrt(v) < 2^34.5 < 2^35. */
    unsigned __int128 low = 0, high = ((unsigned __int128)1 << 35);

    while (low <= high) {
        unsigned __int128 mid = (low + high) >> 1;
        if (mid == 0) {               /* avoid division by zero */
            low = 1;
            continue;
        }
        unsigned __int128 div = v / mid;

        if (mid == div) {             /* mid*mid <= v and mid >= v/mid */
            return (mid * mid == v);  /* final check using 128-bit */
        }
        if (mid < div)
            low = mid + 1;
        else
            high = mid - 1;
    }
    /* high is floor(sqrt(v)) after the loop */
    unsigned __int128 sq = high * high;
    return sq == v;
}

seq_t a(idx_t n)
{
    uint64_t N = n;                    /* n fits in 64 bits */
    for (uint64_t k = 0; ; ++k) {
        uint64_t k2 = k * k;
        if (k2 > N) break;            /* no further candidates */
        uint64_t rem = N - k2;        /* n - k^2 */
        /* v = 8*rem + 1, computed in 128 bits to avoid overflow */
        unsigned __int128 v = (unsigned __int128)rem * 8 + 1;
        if (is_square(v))
            return (seq_t)k;          /* found minimal k */
    }
    return (seq_t)-1;                  /* no k satisfies the condition */
}
