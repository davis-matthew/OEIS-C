/*
 * A221918  Triangle of denominators of sum of two unit fractions: 1/n + 1/m,
 *          n >= m >= 1.
 * https://oeis.org/A221918
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabl, frac
 * Formula:   a(n) = denominator(1/n + 1/m) where the sequence is enumerated
 *            in the triangle a(n,m) with n >= m >= 1, row‑wise: a(1) = 1 a(2)
 *            = 2, a(3) = 1 a(4) = 3, a(5) = 6, a(6) = 3
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"          /* gcd(seq_t, seq_t) */

static idx_t row_for(idx_t n) {
    /* Find the smallest r such that r*(r+1)/2 >= n */
    idx_t lo = 1, hi = 1;
    while ((unsigned __int128)hi * (hi + 1) / 2 < n) hi <<= 1;
    while (lo < hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        if ((unsigned __int128)mid * (mid + 1) / 2 >= n)
            hi = mid;
        else
            lo = mid + 1;
    }
    return lo;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* offset is 1; not used for n==0 */

    /* Determine coordinates (row r, column m) in the triangle */
    idx_t r = row_for(n);                        /* row number (>=1) */
    idx_t prev = (unsigned __int128)(r - 1) * r / 2; /* number of terms before row r */
    idx_t m = n - prev;                          /* column within the row (1..r) */

    /* Compute numerator of n*m/(n+m), i.e. r*m / gcd(r*m, r+m) */
    seq_t nm = (seq_t)r * (seq_t)m;
    seq_t denom = (seq_t)(r + m);
    seq_t g = gcd(nm, denom);
    return nm / g;
}
