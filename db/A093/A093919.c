/*
 * A093919  Consider the triangle in A004736, the k-th term of the n-th row is
 *          the LCM of the preceding k terms.
 * https://oeis.org/A093919
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = lcm of the integers n, n-1, ..., n-k+1 * where r is the
 *            smallest integer satisfying r(r+1)/2 ≥ n, * and k = n –
 *            r(r–1)/2. The sequence is the triangle of these lcms * read
 *            row‑by‑row. The implementation finds r by binary search and *
 *            computes the lcm iteratively
 * Range:     u8 n <= 24, u16 n <= 84, u32 n <= 263, u64 n <= 1099,
 *            u128 n <= 3958, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"

seq_t a(idx_t n) {
    /* find row r such that r(r+1)/2 ≥ n */
    idx_t left = 1, right = n;
    while (left < right) {
        idx_t mid = left + (right - left) / 2;
        /* use 128-bit to avoid overflow for large n */
        __int128 t = (__int128)mid * (mid + 1) / 2;
        if (t >= n) right = mid;
        else left = mid + 1;
    }
    idx_t r = left;

    /* offset inside the row (1‑based) */
    idx_t prev = (r - 1) * r / 2;
    idx_t k = n - prev;          /* 1 ≤ k ≤ r */

    seq_t l = (seq_t)r;          /* lcm of the first element */
    if (k > 1) {
        /* iterate over the remaining k-1 elements */
        for (int64_t i = (int64_t)r - 1; i >= (int64_t)r - (int64_t)k + 1; --i) {
            seq_t term = (seq_t)i;
            seq_t g = gcd(l, term);
            l = l / g * term;   /* lcm(l, term) */
        }
    }
    return l;
}
