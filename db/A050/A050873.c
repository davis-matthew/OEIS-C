/*
 * A050873  Triangular array T read by rows: T(n,k) = gcd(n,k).
 * https://oeis.org/A050873
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, look
 * Formula:   a(n) = gcd(r, k), where r is the row index such that (r-1)*r/2 <
 *            n ≤ r*(r+1)/2 and k = n - (r-1)*r/2. This is simply the n‑th
 *            element of the triangular array T(n,k)=gcd(n,k)
 * Range:     u8 n <= 5050, u16 n <= 5050, u32 n <= 5050, u64 n <= 5050,
 *            u128 n <= 5050, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t, if needed */

/* Helper: Euclidean GCD for idx_t values. */
static inline idx_t gcd_idx(idx_t a, idx_t b)
{
    while (b != 0) {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    /* Row index r: smallest r with r*(r+1)/2 >= n. */
    idx_t r = 1;
    /* Linear search is inexpensive because r ≈ sqrt(2n). */
    while ((r * (r + 1)) / 2 < n)
        ++r;

    /* Compute position k within the row. */
    idx_t prev = ((r - 1) * r) / 2;
    idx_t k = n - prev;               /* 1 ≤ k ≤ r */

    /* GCD of r and k fits in idx_t, cast safely to seq_t. */
    idx_t g = gcd_idx(r, k);
    return (seq_t)g;
}
