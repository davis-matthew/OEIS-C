/*
 * A144473  A triangle sequence of determinants: a(n)=If[Mod[n, 3] == 0, 1,
 *          If[Mod[n, 3] == 1, -1, If[Mod[n, 3] == 2, 0]]]; b(n,m)=If[m < n &&
 *          Mod[n, 3] == 0, 0, If[m < n && Mod[n, 3] == 1, 0, If[m < n &&
 *          Mod[n, 3] == 2 && Mod[n, 2] == 0, 1, If[m < n && Mod[n, 3] == 2 &&
 *          Mod[n, 2] == 1, -1, If[m == n, -1, 0]...
 * https://oeis.org/A144473
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, uned
 * Formula:   a(n) = value at the n-th position of the triangle built from the
 *            determinant t(n,m) = Det[{{a(m), b(n,m)}, {a(n), b(n,n)}}],
 *            where a(k)=If[Mod[k,3]==0,1,If[Mod[k,3]==1,-1,0]] and
 *            b(n,m)=If[m<n && Mod[n,3]==0,0,If[m<n && Mod[n,3]==1,0, If[m<n
 *            && Mod[n,3]==2 && Mod[n,2]==0,1, If[m<n && Mod[n,3]==2...
 * Range:     i8 n <= 55, i16 n <= 55, i32 n <= 55, i64 n <= 55, i128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Helper: integer square root (floor) of a 64‑bit unsigned integer. */
static inline idx_t isqrt_u64(idx_t x)
{
    idx_t low = 0, high = x;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        __int128 prod = (__int128)mid * mid;
        if (prod <= x) {
            low = mid + 1;
        } else {
            if (mid == 0) break;
            high = mid - 1;
        }
    }
    return high;
}

/* Compute the n-th term of the sequence (n >= 1). */
seq_t a(idx_t n)
{
    /* Find the row r such that r*(r+1)/2 >= n. */
    idx_t t = isqrt_u64(1 + 8ULL * n);
    idx_t r = (t - 1) >> 1;            /* floor((t-1)/2)   */
    if (((__int128)r * (r + 1)) / 2 < n) ++r;

    /* Column index within the row (1‑based). */
    idx_t prev = ((__int128)r * (r - 1)) / 2;  /* elements before row r */
    idx_t c = n - prev;                       /* 1 <= c <= r */

    /* The triangle values repeat the pattern -1, 1, 0 for columns 1, 2, 3. */
    switch (c % 3U) {
    case 1: return (seq_t)(-1);
    case 2: return (seq_t)1;
    default:               /* c % 3 == 0 */
        return (seq_t)0;
    }
}
