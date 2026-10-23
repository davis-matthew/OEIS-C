/*
 * A157171  Triangle, read by rows, T(n,k,m) = (m*(n-k)+1)*binomial(n-1, k-1)
 *          + (m*k+1)* binomial(n-1, k) + m*k*(n-k)*binomial(n-2, k-1), with m
 *          = 3.
 * https://oeis.org/A157171
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = value at position n when the triangle with entries
 *            T(n,k,3) = (3*(n-k)+1)*C(n-1,k-1) + (3*k+1)*C(n-1,k) +
 *            3*k*(n-k)*C(n-2,k-1) is read by rows (row 0 has one element, row
 *            1 has two, etc.)
 * Range:     u8 n <= 23, u16 n <= 96, u32 n <= 417, u64 n <= 1737,
 *            u128 n <= 5150, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Assumes the helper function seq_t binomial(idx_t n, idx_t k) is available. */

#include <stdint.h>
#include "../utils/binomial.h"

seq_t a(idx_t n) {
    /* Determine the row r and column k for the global index n. */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n) /* keep r as large as possible */
        ++r;
    idx_t k = n - r * (r + 1) / 2;   /* k in [0, r] */
    
    if (r == 0)                      /* first element of the triangle */
        return (seq_t)1;

    const seq_t m = 3;                /* fixed value m = 3 */
    seq_t result = 0;

    /* First term: (m*(r-k)+1)*C(r-1,k-1)  (valid if r>=1 and k>=1) */
    if (r >= 1 && k >= 1)
        result += (m * (seq_t)(r - k) + (seq_t)1) * binomial(r - 1, k - 1);

    /* Second term: (m*k+1)*C(r-1,k)  (valid if k <= r-1) */
    if (k <= r - 1)   /* r>=1 automatically because r>0 */
        result += (m * (seq_t)k + (seq_t)1) * binomial(r - 1, k);

    /* Third term: m*k*(r-k)*C(r-2,k-1)  (valid if r>=2 and 1<=k<=r-1) */
    if (r >= 2 && k >= 1 && k <= r - 1)
        result += m * (seq_t)k * (seq_t)(r - k) * binomial(r - 2, k - 1);

    return result;
}
