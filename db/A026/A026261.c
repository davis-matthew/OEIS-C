/*
 * A026261  a(n) = j if n = [ j*sqrt(5) ], else a(n) = k if n = [ (k/4)*(5 +
 *          sqrt(5)) ].
 * https://oeis.org/A026261
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the index j or k such that n = ⌊j·√5⌋ or n =
 *            ⌊(k/4)(5+√5)⌋ The implementation finds the index by binary
 *            search using exact integer arithmetic. It uses the identity
 *            ⌊j·√5⌋ = ⌊√(5·j²)⌋ and ⌊(k/4)(5+√5)⌋ = ⌊(5k + √(5·k²))/4⌋,
 *            avoiding floating point
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t isqrt_u128(unsigned __int128 x)
{
    unsigned __int128 low = 0, high = (unsigned __int128)1 << 64;
    while (low + 1 < high) {
        unsigned __int128 mid = (low + high) >> 1;
        if (mid * mid <= x)
            low = mid;
        else
            high = mid;
    }
    return (seq_t)low;
}

seq_t a(idx_t n)
{
    /* First try the Beatty sequence floor(j*sqrt(5)) */
    idx_t lo = 1, hi = n;
    while (lo <= hi) {
        idx_t mid = (lo + hi) >> 1;
        unsigned __int128 val = (unsigned __int128)mid * mid * 5;
        seq_t sq = isqrt_u128(val);
        if (sq == n)
            return (seq_t)mid;
        if (sq < n)
            lo = mid + 1;
        else
            hi = mid - 1;
    }

    /* Second, try the sequence floor((k/4)(5+sqrt(5))) */
    lo = 1;
    hi = n;
    while (lo <= hi) {
        idx_t mid = (lo + hi) >> 1;
        unsigned __int128 val = (unsigned __int128)mid * mid * 5;
        seq_t sq = isqrt_u128(val);
        seq_t cand = (( (unsigned __int128)5 * mid + sq ) ) / 4;
        if (cand == n)
            return (seq_t)mid;
        if (cand < n)
            lo = mid + 1;
        else
            hi = mid - 1;
    }

    /* According to the definition, every n >= 1 should match
       one of the two sequences; reaching here indicates an
       unexpected error. */
    return 0;
}
