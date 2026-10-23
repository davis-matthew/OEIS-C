/*
 * A022838  Beatty sequence for sqrt(3); complement of A054406.
 * https://oeis.org/A022838
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(n*sqrt(3))
 * Range:     u8 n <= 147, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* out‑of‑range fallback */

    unsigned __int128 nn = (unsigned __int128)n;
    unsigned __int128 target = nn * nn * 3;          /* 3 * n^2 */

    /* upper bound 2*n, truncated to 64‑bit maximum */
    unsigned __int128 hi128 = nn * 2;
    uint64_t hi = (hi128 > (unsigned __int128)UINT64_MAX)
                  ? UINT64_MAX
                  : (uint64_t)hi128;
    uint64_t lo = 0;

    while (lo <= hi) {
        uint64_t mid = lo + ((hi - lo) >> 1);
        unsigned __int128 mid2 = (unsigned __int128)mid * mid;

        if (mid2 <= target)
            lo = mid + 1;
        else
            hi = mid - 1;
    }

    return (seq_t)hi;   /* floor(sqrt(3*n*n)) == floor(n*sqrt(3)) */
}
