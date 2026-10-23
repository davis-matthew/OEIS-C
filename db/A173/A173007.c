/*
 * A173007  Triangle T(n,k) read by rows: coefficient [x^k] of the polynomial
 *          Product_{i=1..n} (x + q^i) in row n and q = 3.
 * https://oeis.org/A173007
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) enumerates the triangle T(r, k) with q = 3, where r = max {
 *            m | m*(m+1)/2 ≤ n } and k = n – r*(r+1)/2. The triangle
 *            satisfies T(0,0) = 1, T(r, k) = 3^r * T(r‑1, k) + T(r‑1, k‑1).
 *            a(n) = T(r, k)
 * Range:     u8 n <= 5, u16 n <= 14, u32 n <= 20, u64 n <= 44, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Find row r such that r*(r+1)/2 ≤ n < (r+1)*(r+2)/2        */
    idx_t low = 0, high = n, r = 0;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        unsigned __int128 t = (__int128)mid * (mid + 1) / 2;
        if (t <= n) {
            r = mid;
            low = mid + 1;
        } else {
            if (mid == 0) break;
            high = mid - 1;
        }
    }

    /* Column index within the row   */
    idx_t tri   = (idx_t)(((__int128)r * (r + 1)) / 2);
    idx_t k     = n - tri;

    /* Allocate memory for the current row (size r+1)             */
    seq_t *row = (seq_t *)malloc((r + 1) * sizeof(seq_t));
    if (!row) return 0;                     /* allocation failed  */

    /* Initialize the row array to zero */
    for (idx_t i = 0; i <= r; ++i) row[i] = 0;
    row[0] = 1;                             /* base T(0,0)       */

    /* Compute rows 1 … r                          */
    seq_t pow3 = 1;                         /* current 3^cur      */
    for (idx_t cur = 1; cur <= r; ++cur) {
        pow3 *= 3;                           /* 3^cur              */
        for (idx_t idx = cur; idx > 0; --idx) {
            seq_t val = pow3 * row[idx];     /* 3^cur * T(cur-1,idx)  */
            val += row[idx - 1];             /* + T(cur-1,idx-1)     */
            row[idx] = val;
        }
        /* New element at index 0                 */
        row[0] = pow3 * row[0];              /* 3^cur * T(cur-1,0)   */
    }

    seq_t result = row[k];
    free(row);
    return result;
}
