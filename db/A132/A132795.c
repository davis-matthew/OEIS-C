/*
 * A132795  Triangle of Gely numbers, read by rows.
 * https://oeis.org/A132795
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) = T(row, col) where T(n,k) = Σ_{j=0..k} (-1)^j * C(n+1,j) *
 *            Σ_{m=0..n} (k-j)^m. The triangle is read by rows (offset 0);
 *            a(0)=T(0,0), a(1)=T(1,0), a(2)=T(1,1), …
 * Range:     u8 n <= 23, u16 n <= 48, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Helper: binary search for row index r such that r(r+1)/2 <= n < (r+1)(r+2)/2. */
static idx_t find_row(idx_t n)
{
    idx_t lo = 0, hi = 1;
    /* increase hi until it is large enough */
    while ((__int128)hi * (hi + 1) <= (__int128)2 * n) hi <<= 1;
    while (lo < hi) {
        idx_t mid = lo + (hi - lo + 1) / 2;
        /* mid(mid+1)/2 <= n  ->  mid*(mid+1) <= 2*n  */
        if ((__int128)mid * (mid + 1) <= (__int128)2 * n)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

/* Compute the triangle entry T(n,k). */
static seq_t triangle_value(idx_t n, idx_t k)
{
    seq_t sum_pos = 0;
    seq_t sum_neg = 0;
    seq_t binom = 1;                 /* C(n+1,0) */
    for (idx_t j = 0; j <= k; ++j) {
        /* Sum of powers Σ_{m=0..n} (k-j)^m */
        seq_t base = (k > j) ? (seq_t)(k - j) : (seq_t)0;
        seq_t sumpow = 1;            /* term for m=0 */
        if (k > j) {
            seq_t term = 1;
            for (idx_t m = 1; m <= n; ++m) {
                term *= base;
                sumpow += term;
            }
        }
        /* term_j = binom * sumpow */
        seq_t term = binom * sumpow;
        if ((j & 1) == 0)
            sum_pos += term;
        else
            sum_neg += term;

        /* Update binom for next j: C(n+1, j+1) = C(n+1, j) * (n+1-j)/(j+1) */
        if (j < k) {
            binom = binom * (seq_t)(n + 1 - j) / (j + 1);
        }
    }
    return sum_pos - sum_neg;
}

/* External function required by the build system. */
seq_t a(idx_t n)
{
    /* Determine (row, col) = (r, k) for index n. */
    idx_t r = find_row(n);
    idx_t k = n - r * (r + 1) / 2;
    return triangle_value(r, k);
}
