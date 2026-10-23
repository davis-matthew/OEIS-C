/*
 * A156699  Triangle T(n, k) = Product_{j=1..k} Product_{i=0..j-1} ( 1 -
 *          (n-k+1)*(2*i-1) ) with T(n, 0) = 1 and T(n, n) = n!, read by rows.
 * https://oeis.org/A156699
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   T(n,k) = ∏_{j=1..k} ∏_{i=0..j-1} ( 1 - (n-k+1)*(2*i-1) ), with
 *            T(n,0)=1, T(n,n)=n!; a(n) is the sequence read by rows (offset
 *            0)
 * Range:     i8 n <= 12, i16 n <= 24, i32 n <= 32, i64 n <= 50, i128 n <= 74,
 *            big any n
 * Helpers:   factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/factorial.h"

seq_t a(idx_t n)
{
    /* Find the row r such that r*(r+1)/2 ≤ n < (r+1)*(r+2)/2. */
    idx_t r = 0;
    while (((idx_t)r * (r + 1)) / 2 <= n) {
        ++r;
    }
    --r;  /* now r*(r+1)/2 ≤ n */

    idx_t start = ((idx_t)r * (r + 1)) / 2;  /* first index of this row */
    idx_t k = n - start;                     /* column within the row */

    /* Handle trivial cases. */
    if (k == 0)        return 1;               /* T(r,0) = 1                  */
    if (k == r)        return factorial(r);    /* T(r,r) = r!                 */

    /* General product for 0 < k < r. */
    seq_t mul_const = (seq_t)(r - k + 1);      /* n-k+1 (here n = r)          */
    seq_t product   = 1;

    for (idx_t j = 1; j <= k; ++j) {
        for (idx_t i = 0; i < j; ++i) {
            /* compute 1 - mul_const * (2*i - 1) with signed arithmetic  */
            int64_t two_i_minus_one = (int64_t)2 * (int64_t)i - 1;
            seq_t factor = 1 - mul_const * (seq_t)two_i_minus_one;
            product *= factor;
        }
    }

    return product;
}
