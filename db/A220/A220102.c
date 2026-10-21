/*
 * A220102  Permutation of natural numbers arising from applying the walk of
 *          square spiral (e.g. A214526) to the data of double square spiral
 *          (defined in A220098).
 * https://oeis.org/A220102
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the value at the n-th position of the standard *
 *            square‑spiral walk read from the data of the double square
 *            spiral. * The value is computed by the following closed‑form
 *            algorithm * described in the analysis
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* integer square‑root of a 64‑bit unsigned integer */
static inline idx_t
isqrt_u64(idx_t n)
{
    idx_t lo = 0, hi = n;
    while (lo <= hi) {
        idx_t mid = lo + (hi - lo) / 2;
        __int128 sq = (__int128)mid * mid;
        if (sq == n) return mid;
        if (sq < n)
            lo = mid + 1;
        else
            hi = mid - 1;
    }
    return hi;               /* floor(sqrt(n)) */
}

seq_t
a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;

    /* find ring r: minimal r >= 1 with 1 + 4*r*(r+1) >= n */
    idx_t r = (isqrt_u64(n) - 1) / 2;
    if (( ( __int128)1 + (__int128)4 * (__int128)r * (r + 1) ) < n )
        ++r;

    /* start value of this ring */
    idx_t start = 2 + 4 * (r - 1) * r;

    /* number of elements before this ring */
    idx_t before = 1 + 4 * (r - 1) * r;

    /* index within the ring, 0‑based */
    idx_t i = n - before - 1;          /* 0 ≤ i < 8r */

    /* compute the value in the ring */
    if ((r & 1) == 1) { /* odd ring: evens first, then odds */
        if (i < 4 * r)
            return (seq_t)((__int128)start + 2 * (__int128)i);
        else
            return (seq_t)((__int128)start + 1 + 2 * (__int128)(i - 4 * r));
    } else {                /* even ring: odds first, then evens */
        if (i < 4 * r)
            return (seq_t)((__int128)start + 1 + 2 * (__int128)i);
        else
            return (seq_t)((__int128)start + 2 * (__int128)(i - 4 * r));
    }
}
