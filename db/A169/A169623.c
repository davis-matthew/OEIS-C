/*
 * A169623  Generalized Pascal triangle read by rows: T(n,0) = T(0,n) = 1 for
 *          n >= 0, T(n,k) = 0 for k < 0 or k > n; otherwise T(n,k) =
 *          T(n-2,k-2) + T(n-2,k-1) + T(n-2,k) for 1 <= k <= n-1.
 * https://oeis.org/A169623
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) = value of the generalized Pascal triangle T(n,k) read by
 *            rows, where T(n,0) = T(0,n) = 1, T(n,k) = 0 for k<0 or k>n,
 *            T(n,k) = T(n-2,k-2) + T(n-2,k-1) + T(n-2,k) for 1 ≤ k ≤ n-1
 * Range:     u8 n <= 96, u16 n <= 310, u32 n <= 1053, u64 n <= 3779,
 *            u128 n <= 9708, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Special case */
    if (n == 0) return (seq_t)1;

    /* Work arrays for even and odd rows, maximum needed length 200 */
    seq_t even[200];
    seq_t odd[200];
    size_t even_len = 1;          /* row 0 */
    size_t odd_len  = 2;          /* row 1 */
    even[0] = (seq_t)1;
    odd[0] = odd[1] = (seq_t)1;

    size_t cumulative = 0;
    /* Row 0 */
    if (n < cumulative + 1) return even[0];
    cumulative += 1;
    /* Row 1 */
    if (n < cumulative + 2) return odd[n - cumulative];
    cumulative += 2;

    for (idx_t r = 2; ; ++r) {
        size_t len = r + 1;               /* length of current row */
        seq_t new_row[200];
        new_row[0] = (seq_t)1;
        new_row[len - 1] = (seq_t)1;

        /* Compute interior entries */
        for (size_t i = 1; i < r; ++i) {
            seq_t sum = 0;
            if (r % 2 == 0) {            /* even row uses previous even */
                if (i - 2 < even_len) sum += even[i - 2];
                if (i - 1 < even_len) sum += even[i - 1];
                if (i < even_len)     sum += even[i];
            } else {                     /* odd row uses previous odd */
                if (i - 2 < odd_len)  sum += odd[i - 2];
                if (i - 1 < odd_len)  sum += odd[i - 1];
                if (i < odd_len)      sum += odd[i];
            }
            new_row[i] = sum;
        }

        /* Store the newly computed row */
        if (r % 2 == 0) {
            for (size_t j = 0; j < len; ++j) even[j] = new_row[j];
            even_len = len;
        } else {
            for (size_t j = 0; j < len; ++j) odd[j] = new_row[j];
            odd_len = len;
        }

        /* Check if the requested index falls in this row */
        if (n < cumulative + len) {
            return (r % 2 == 0) ? even[n - cumulative] : odd[n - cumulative];
        }
        cumulative += len;
    }
}
