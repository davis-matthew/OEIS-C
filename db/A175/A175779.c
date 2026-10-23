/*
 * A175779  Triangle T(n,m) read by rows: numerator of 1/(n-m)^2 - 1/n^2.
 * https://oeis.org/A175779
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, tabl
 * Formula:   a(n) = flattened triangle T(n,m) where * 0 ≤ m ≤ n, T(n,0)=0 for
 *            n>0, T(n,n)=1, * otherwise T(n,m) = numerator of 1/(n‑m)² − 1/n²
 *            reduced to lowest terms
 * Range:     u8 n <= 15, u16 n <= 543, u32 n <= 5150, u64 n <= 5150,
 *            u128 n <= 5150, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Euclidean algorithm for seq_t (unsigned). */
static inline seq_t seq_gcd_local(seq_t a, seq_t b)
{
    while (b != 0) {
        seq_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* Find the row r such that triangular(r) ≤ n < triangular(r+1). */
static inline idx_t find_row(idx_t n)
{
    idx_t lo = 0, hi = 1;
    /* Increase hi until triangular(hi) > n. */
    while ((hi * (hi + 1) / 2) <= n) {
        hi <<= 1;
    }
    /* Binary search between lo and hi. */
    while (lo + 1 < hi) {
        idx_t mid = (lo + hi) >> 1;
        if ((mid * (mid + 1) / 2) <= n)
            lo = mid;
        else
            hi = mid;
    }
    return lo;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    idx_t r = find_row(n);                     /* row */
    idx_t m = n - r * (r + 1) / 2;             /* column */

    /* Handle the special cases. */
    if (r == 0 && m == 0)                      /* T(0,0) = 1 */
        return (seq_t)1;
    if (m == 0)                                /* T(n,0) = 0 for n>0 */
        return (seq_t)0;
    if (m == r)                                /* T(n,n) = 1 at the pole */
        return (seq_t)1;

    /* Compute numerator and denominator of the fraction. */
    seq_t num = (seq_t)m * (seq_t)(2 * r - m);
    seq_t denom = (seq_t)r * r * (seq_t)(r - m) * (r - m);

    /* Reduce to lowest terms. */
    seq_t g = seq_gcd_local(num, denom);
    return num / g;
}
