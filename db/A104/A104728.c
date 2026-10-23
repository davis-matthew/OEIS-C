/*
 * A104728  Triangle T(n,k) = (k-1-n)*(k-2-n)*(k-2+2*n)/2 read by rows,
 *          1<=k<=n.
 * https://oeis.org/A104728
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   not stated
 * Range:     u8 n <= 21, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS: A104728 – Triangle read by rows
 * Formula: a(n) = (k-1-n)*(k-2-n)*(k-2+2*n)/2
 * where n is the row of the triangle and
 * k = n - T_{n-1} is the column index (T_m=m(m+1)/2).
 * The function returns the n‑th term in row‑major order (offset 1).
 */

#include <stdint.h>
#include <inttypes.h>

/* Find the row r (1‑based) such that T_{r-1} < n ≤ T_r   */
static idx_t find_row(idx_t n)
{
    idx_t low = 0, high = 1;
    /* Exponential search for a high bound */
    while ((high * (high + 1)) / 2 < n)
        high <<= 1;

    /* Binary search between low and high */
    while (low + 1 < high) {
        idx_t mid = low + ((high - low) >> 1);
        if ((mid * (mid + 1)) / 2 >= n)
            high = mid;
        else
            low = mid;
    }
    return high;
}

seq_t a(idx_t n)
{
    /* 1 ≤ n.  For n < 1, the offset is 1 so behaviour is undefined. */
    idx_t r = find_row(n);                        /* row index (1‑based)   */
    idx_t tri_prev = (r - 1) * r / 2;              /* T_{r-1} = (r-1)r/2     */
    idx_t k = n - tri_prev;                       /* column index in row   */

    /* Use signed arithmetic for the intermediate negative factors */
    int64_t term1 = (int64_t)(k - 1) - (int64_t)r;
    int64_t term2 = (int64_t)(k - 2) - (int64_t)r;
    int64_t term3 = (int64_t)(k - 2) + (int64_t)(2 * r);

    int64_t product = term1 * term2 * term3;
    /* The product is even, so division by 2 is exact. */
    return (seq_t)(product / 2);
}
