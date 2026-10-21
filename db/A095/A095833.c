/*
 * A095833  Triangle read by rows: T(n,k) = (n-k+1)*n, n>=1, 1<=k<=n.
 * https://oeis.org/A095833
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = r*(r - k + 1), where r = smallest integer with r*(r+1)/2
 *            ≥ n and * k = n - r*(r-1)/2 is the column index inside that row
 *            (1‑based)
 * Range:     u8 n <= 44, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;            /* defined only for n ≥ 1 */

    /* Find r by binary search on the range [1, n]. */
    idx_t low = 1, high = n;
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        seq_t sum_mid = (seq_t)mid * ((seq_t)mid + 1) / 2;
        if (sum_mid >= (seq_t)n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t r = low;                    /* row number */

    /* start index of row r */
    seq_t start_row = ((seq_t)(r - 1) * (seq_t)r / 2) + 1;
    idx_t offset = n - (idx_t)start_row;   /* 0‑based column position in the row */

    /* a(n) = r * (r - offset) because offset = k-1 */
    seq_t value = (seq_t)r * ((seq_t)r - (seq_t)offset);
    return value;
}
