/*
 * A152570  Triangle T(n,k) read by rows: T(n,n) = -1, T(n,0) = 3^(n - 1),
 *          T(n,k) = -3^(n - k - 1), 1 <= k <= n - 1.
 * https://oeis.org/A152570
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, tabl
 * Formula:   a(n) = T(r,k) where r is the row index and k the column index: *
 *            T(r,0) = 3^(r-1) (r > 0) * T(r,r) = -1 * T(r,k) = -3^(r-k-1) (1
 *            ≤ k ≤ r-1) * For each n ≥ 0 find r such that r*(r+1)/2 ≤ n <
 *            (r+1)*(r+2)/2 * and set k = n - r*(r+1)/2. Then a(n) = T(r,k)
 * Range:     i8 n <= 20, i16 n <= 57, i32 n <= 57, i64 n <= 57, i128 n <= 57,
 *            big any n
 * Helpers:   pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow3.h"

seq_t a(idx_t n)
{
    /* Special case for the very first element of the triangle */
    if (n == 0)
        return (seq_t)-1;

    idx_t r = 0;
    idx_t cumulative = 0;   /* start index of the current row */

    /* Find the row r containing the linear index n.
       After the loop: cumulative = r*(r+1)/2  (start of row r) */
    while (n >= cumulative + (r + 1)) {
        cumulative += (r + 1);
        r++;
    }

    idx_t k = n - cumulative;   /* column within the row */

    if (k == 0)                 /* first element of the row (r > 0) */
        return pow3(r - 1);

    if (k == r)                 /* last element of the row */
        return (seq_t)-1;

    /* middle element */
    return -(pow3(r - k - 1));
}
