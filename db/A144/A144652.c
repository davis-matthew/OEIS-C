/*
 * A144652  Triangle, read by rows, where T(m,n) = floor((2mn+m+n)/2) with m
 *          >= n >= 1.
 * https://oeis.org/A144652
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = floor((2*m*k + m + k)/2), where m is the smallest integer
 *            with m*(m+1)/2 ≥ n and k = n - m*(m-1)/2 (1 ≤ k ≤ m). The
 *            triangle is read by rows
 * Range:     u8 n <= 65, u16 n <= 5050, u32 n <= 5050, u64 n <= 5050,
 *            u128 n <= 5050, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Computes the n-th term (n ≥ 1) of OEIS A144652. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* undefined, but prevent underflow */

    /* Find the row m such that cumulative count up to row m ≥ n */
    idx_t m   = 1;   /* current row number */
    idx_t cum = 1;   /* cumulative terms = m*(m+1)/2 for current m */

    while (cum < n) {
        ++m;
        cum += m;   /* cum now equals triangular number for row m */
    }

    /* Position k within the found row m (1‑based) */
    idx_t k = n - (cum - m);

    /* Compute T(m,k) = floor((2*m*k + m + k)/2) */
    seq_t numerator = ((seq_t)2 * (seq_t)m * (seq_t)k)
                     + (seq_t)m + (seq_t)k;
    return numerator >> 1;
}
