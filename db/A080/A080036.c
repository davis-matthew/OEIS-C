/*
 * A080036  a(n) = n + round(sqrt(2*n)) + 1.
 * https://oeis.org/A080036
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, changed
 * Formula:   a(n) = n + round(sqrt(2*n)) + 1
 * Range:     u8 n <= 232, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 90 terms through n = 872 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* for n≥0, with a(0)=1, a(1)=3, … */
#include <stdint.h>

static inline seq_t round_sqrt2(idx_t n)
{
    /* compute floor(sqrt(2*n)) and then round to nearest integer */
    unsigned __int128 m = ((unsigned __int128)n) << 1;      /* 2*n   */
    uint64_t lo = 0;
    uint64_t hi = ((uint64_t)1) << 33;                  /* sqrt(2*n) < 2^33 */

    /* binary search for floor sqrt */
    while (lo + 1 < hi) {
        uint64_t mid = lo + ((hi - lo) >> 1);
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq <= m) {
            lo = mid;
        } else {
            hi = mid;
        }
    }

    /* now lo = floor sqrt(2*n) */
    unsigned __int128 lo_sq = (unsigned __int128)lo * lo;
    unsigned __int128 next_sq = (unsigned __int128)(lo + 1) * (lo + 1);

    unsigned __int128 diff = m - lo_sq;
    unsigned __int128 next_diff = next_sq - m;

    uint64_t rounded = (diff < next_diff) ? lo : lo + 1;
    return (seq_t)rounded;
}

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t rs = round_sqrt2(n);
    return nn + rs + (seq_t)1;
}
