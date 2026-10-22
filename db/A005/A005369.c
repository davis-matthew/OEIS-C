/*
 * A005369  a(n) = 1 if n is of the form m(m+1), else 0.
 * https://oeis.org/A005369
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   a(n) = 1 if n = m*(m+1) for some m >= 0, else 0; find m =
 *            largest integer with m*(m+1) <= n by binary search, then test
 *            equality
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    uint64_t lo = 0, hi = 4294967296ULL;
    while (lo < hi) {
        uint64_t mid = lo + (hi - lo + 1) / 2;
        unsigned __int128 p = (unsigned __int128)mid * ((unsigned __int128)mid + 1);
        if (p <= (unsigned __int128)n)
            lo = mid;
        else
            hi = mid - 1;
    }
    unsigned __int128 q = (unsigned __int128)lo * ((unsigned __int128)lo + 1);
    seq_t r = 0;
    if (q == (unsigned __int128)n)
        r = 1;
    return r;
}
