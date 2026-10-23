/*
 * A153878  A triangular sequence recursion: A(n,k)=A(n - 1, k - 1) + A(n - 1,
 *          k) + (-12 + 5 n) (-9 + 5 n)*A(n - 2, k - 1).
 * https://oeis.org/A153878
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   A(n,k)=A(n-1,k-1)+A(n-1,k)+(-12+5 n)(-9+5 n)*A(n-2,k-1) with
 *            initial rows A(1,1)=2 Row 2: {3,3} Row 3: {2,44,2} a(n) is the
 *            flattened sequence of this triangle (offset 1)
 * Range:     u8 n <= 7, u16 n <= 17, u32 n <= 40, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Maximum number of entries needed for n up to about 10000
       (141 rows give 100... entries). */
    static seq_t triangle[20000] = {0};
    static idx_t max_row_computed = 0;

    if (n < 1)
        return (seq_t)0;   /* undefined for n < 1 */

    /* Determine the row r (1‑based) and column k (1‑based) of the
       requested flattened entry.  The cumulative count of entries
       up to row r is r*(r+1)/2. */
    idx_t r = 1;
    while ((r * (r + 1)) / 2 < n)
        ++r;

    idx_t k = n - (r * (r - 1)) / 2;

    /* Initialise base rows on first call. */
    if (max_row_computed == 0)
    {
        triangle[0] = (seq_t)2;          /* Row 1 */
        triangle[1] = (seq_t)3;          /* Row 2 */
        triangle[2] = (seq_t)3;
        triangle[3] = (seq_t)2;          /* Row 3 */
        triangle[4] = (seq_t)44;
        triangle[5] = (seq_t)2;
        max_row_computed = 3;
    }

    /* Build further rows on demand; recurrence is valid for n ≥ 4. */
    while (r > max_row_computed)
    {
        idx_t nrow = max_row_computed + 1;        /* row to compute */

        /* Starting indices of rows n-1 and n-2 in the flat array. */
        idx_t start_prev1 = (nrow - 1) * (nrow - 2) / 2;
        idx_t start_prev2 = (nrow - 2) * (nrow - 3) / 2;
        idx_t start_curr   = nrow * (nrow - 1) / 2;

        /* Coefficient (-12+5n)(-9+5n). */
        seq_t coeff = ((seq_t)5 * (seq_t)nrow - 12) *
                       (((seq_t)5 * (seq_t)nrow) - 9);

        /* Compute each entry of the new row. */
        for (idx_t col = 1; col <= nrow; ++col)
        {
            seq_t a_val = (col >= 2)
                            ? triangle[(size_t)(start_prev1 + col - 2)]
                            : (seq_t)0;

            seq_t b_val = (col <= nrow - 1)
                            ? triangle[(size_t)(start_prev1 + col - 1)]
                            : (seq_t)0;

            seq_t c_val = (col >= 2 && col <= nrow - 1)
                            ? triangle[(size_t)(start_prev2 + col - 2)]
                            : (seq_t)0;

            triangle[(size_t)(start_curr + col - 1)] =
                a_val + b_val + coeff * c_val;
        }

        ++max_row_computed;
    }

    /* Return the requested term. */
    idx_t start = r * (r - 1) / 2;
    return triangle[(size_t)(start + k - 1)];
}
