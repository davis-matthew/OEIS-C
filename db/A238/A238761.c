/*
 * A238761  Subtriangle of the generalized ballot numbers, T(n,k) =
 *          A238762(2*k-1,2*n-1), 1<=k<=n, read by rows.
 * https://oeis.org/A238761
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   T(n,k) = (n-k+1) * C(n+k-1, k-1) (for 1 ≤ k ≤ n) a(n) is the nth
 *            term of the flattened triangle T(n,k)
 * Range:     u8 n <= 14, u16 n <= 43, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Find the row r such that r(r+1)/2 ≥ n.  */
    idx_t low = 1, high = 1;
    while (high * (high + 1) / 2 < n) high *= 2;     /* exponential increase */
    while (low < high) {
        idx_t mid = (low + high) / 2;
        if (mid * (mid + 1) / 2 >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t r = low;                                 /* row number */

    /* column c (1‑based) in that row. */
    idx_t sum_before = (r - 1) * r / 2;
    idx_t c = n - sum_before;

    /* Compute T(r,c) = (r-c+1) * C(r+c-1, c-1). */
    seq_t factor = (seq_t)(r - c + 1);
    seq_t comb   = binomial(r + c - 1, c - 1); /* uses utility */
    return factor * comb;
}
