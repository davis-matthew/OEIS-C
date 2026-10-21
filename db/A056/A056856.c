/*
 * A056856  Triangle of numbers related to rooted trees and unrooted planar
 *          trees.
 * https://oeis.org/A056856
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) is the nth element in the triangle defined by ∑_{k=0}^{n-1}
 *            T(n,k) y^k = ∏_{k=1}^{n-1} (k + n y). The row‑major flattening
 *            gives a(1)=1, a(2)=1, a(3)=2, a(4)=2, a(5)=9, …. This
 *            implementation finds the row r and column c of the nth element
 *            and evaluates T(r,c) = r^c · e_{r-1-c}(1,2,…,r-1), w...
 * Range:     u8 n <= 12, u16 n <= 23, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined, guard */

    /* Find the row r such that r*(r+1)/2 >= n. Use a running sum to avoid overflow. */
    idx_t r = 1;
    idx_t sum = 1;                  /* cumulative sum up to current r */
    while (sum < n) {
        ++r;
        sum += r;
    }

    /* previous cumulative sum (rows 1..r-1) */
    idx_t prev = sum - r;
    /* column index in row r (0‑based) */
    idx_t col = n - prev - 1;

    /* Compute elementary symmetric sums e[0..r-1] for the set {1,2,…,r-1}. */
    seq_t *e = (seq_t *)malloc(r * sizeof(seq_t));
    if (!e) return 0;               /* allocation failure */
    e[0] = 1;
    for (idx_t j = 1; j < r; ++j) e[j] = 0;

    for (idx_t i = 1; i < r; ++i) {
        for (idx_t j = i; j >= 1; --j) {
            e[j] = e[j] + (seq_t)i * e[j - 1];
        }
    }

    /* e_val = e_{r-1-col} */
    seq_t e_val = e[r - 1 - col];

    /* Compute r^col */
    seq_t pow = 1;
    for (idx_t i = 0; i < col; ++i) {
        pow *= (seq_t)r;
    }

    seq_t result = pow * e_val;

    free(e);
    return result;
}
