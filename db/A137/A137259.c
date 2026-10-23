/*
 * A137259  Triangle T(n, k) = n*( (n-1)! - (k-1)! ), read by rows.
 * https://oeis.org/A137259
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = T(r,k) where r = row containing flat index n (rows are
 *            counted starting at 1), k = (n - r(r-1)/2 – 1) + 1 (position of
 *            n within that row), T(r,k) = r * ( (r-1)! - (k-1)! ). The
 *            function uses the provided factorial() helper
 * Range:     u8 n <= 15, u16 n <= 36, u32 n <= 78, u64 n <= 210,
 *            u128 n <= 595, big any n
 * Helpers:   factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/factorial.h"

/* Return the row r for a given flat index n ≥ 1. */
static idx_t triangular_row(idx_t n)
{
    idx_t low = 1, high = n;          /* high is always enough for n ≥ 1 */
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        idx_t tri = mid * (mid + 1) / 2;
        if (tri >= n)
            high = mid;
        else
            low = mid + 1;
    }
    return low;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined for the offset 1 case */
    idx_t r = triangular_row(n);          /* row number */

    /* first index in this row: 1 + (r-1)*r/2 */
    idx_t first = (r - 1) * r / 2 + 1;
    idx_t k = n - first + 1;              /* 1 ≤ k ≤ r */

    seq_t rfact   = factorial(r);          /* r!      */
    seq_t fact_k1 = factorial(k - 1);      /* (k-1)!  */
    seq_t mult    = (seq_t)r * fact_k1;    /* r*(k-1)! */

    return rfact - mult;                   /* T(r,k)  */
}
