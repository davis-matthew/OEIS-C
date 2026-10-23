/*
 * A010906  Inverse Aronson transform of squares.
 * https://oeis.org/A010906
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) enumerates integers according to the segments described in
 *            OEIS A010906. For k>=3, if k is a square we add k^2; otherwise
 *            we add all integers from (k-1)^2+1 up to k^2-1. The special
 *            cases are a(0)=0, a(1)=1, a(2)=3
 * Range:     u8 n <= 71, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Integer square root (floor) for 64‑bit values. */
static idx_t isqrt_u64(idx_t x)
{
    idx_t low = 0;
    idx_t high = ((idx_t)1ULL << 32);  /* > sqrt(2^64-1) */
    while (low < high) {
        idx_t mid = low + ((high - low) >> 1);
        __int128 prod = (__int128)mid * mid;
        if (prod == x) return mid;
        if (prod < x)
            low = mid + 1;
        else
            high = mid;
    }
    return low - 1;
}

seq_t a(idx_t n)
{
    idx_t idx = 0;   /* current index in the sequence */

    /* segment k = 0 */
    if (idx == n) return (seq_t)0;
    idx++;

    /* segment k = 1 */
    if (idx == n) return (seq_t)1;
    idx++;

    /* segment k = 2  (exception: contains only 3) */
    if (idx == n) return (seq_t)3;
    idx++;

    /* k >= 3 */
    for (idx_t k = 3; ; ++k) {
        idx_t root = isqrt_u64(k);
        if (root * root == k) {        /* k is a square */
            seq_t val = (seq_t)k * (seq_t)k;
            if (idx == n) return val;
            idx++;
        } else {                       /* k is not a square */
            seq_t start = (seq_t)(k - 1) * (seq_t)(k - 1) + 1;
            seq_t end   = (seq_t)k * (seq_t)k - 1;
            for (seq_t x = start; x <= end; ++x) {
                if (idx == n) return x;
                idx++;
            }
        }
    }
    /* unreachable, but required to satisfy compiler */
    return (seq_t)0;
}
