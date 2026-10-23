/*
 * A180570  Triangle read by rows: T(n,k) is the number of unordered pairs of
 *          vertices at distance k in the graph \|/_\/_\/_..._\/_\|/ having n
 *          nodes on the horizontal path. The entries in row n are the
 *          coefficients of the Wiener polynomial of the graph.
 * https://oeis.org/A180570
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = T(r,k) where r is the row such that the offset of row r
 *            <= n < offset of row r+1, and k = n - offset(r) + 1. T(r,k) is
 *            given by T(r,1) = 1 + 3r T(r,2) = 6r T(r,3) = 9(r-1) T(r,4) =
 *            9(r-2) T(r,k) = 9(r+2-k) for 5 <= k <= r+1 (row r has r+1
 *            entries)
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Compute integer floor of sqrt of a 128-bit value represented by
   (unsigned __int128) x. */
static unsigned long long floor_isqrt_u128(unsigned __int128 x) {
    unsigned long long low = 0, high = ((unsigned long long)1) << 32;
    while (low <= high) {
        unsigned long long mid = (low + high) >> 1;
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq <= x) {
            low = mid + 1;
        } else {
            if (mid == 0) break;
            high = mid - 1;
        }
    }
    return high;
}

seq_t a(idx_t n)
{
    /* For indices below the offset (2) the behavior is irrelevant. */
    if (n < 2) return (seq_t)0;

    /*   offset(r) = 2 + (r-2)(r+3)/2
     *   Let i = r-2  =>  offset = 2 + i(i+5)/2
     *   Find largest i >= 0 such that i(i+5)/2 <= n-2
     */
    idx_t t = n - 2; /* non‑negative */
    /* Solve i^2 + 5i - 2t <= 0  ->  i = floor( (-5 + sqrt(25 + 8t)) / 2 ) */
    unsigned __int128 discr = (unsigned __int128)25 + (unsigned __int128)8 * t;
    unsigned long long sqrt_discr = floor_isqrt_u128(discr);
    unsigned long long i = 0;
    if (sqrt_discr > 5) i = (sqrt_discr - 5) >> 1; /* divide by 2 */
    else i = 0;
    /* adjust if overestimated */
    while (((unsigned __int128)i * (i + (unsigned long long)5) / 2) > t) i--;
    while (((unsigned __int128)(i + 1) * (i + 1 + (unsigned long long)5) / 2) <= t) i++;

    idx_t row = i + 2;  /* r = i+2 */
    idx_t offset = 2 + (idx_t)((unsigned __int128)i * (i + (unsigned long long)5) / 2);
    idx_t k = n - offset + 1;  /* position inside the row, 1‑based */

    /* Compute T(row,k) using the piecewise formula. */
    if (k == (idx_t)1) {
        return (seq_t)1 + (seq_t)3 * (seq_t)row;
    } else if (k == (idx_t)2) {
        return (seq_t)6 * (seq_t)row;
    } else if (k == (idx_t)3) {
        return (seq_t)9 * ((seq_t)row - (seq_t)1);
    } else if (k == (idx_t)4) {
        return (seq_t)9 * ((seq_t)row - (seq_t)2);
    } else {
        /* 5 <= k <= row+1 */
        return (seq_t)9 * (((seq_t)row + (seq_t)2) - (seq_t)k);
    }
}
