/*
 * A062980  a(0) = 1, a(1) = 5; for n > 1, a(n) = 6*n*a(n-1) + Sum_{k=1..n-2}
 *          a(k)*a(n-k-1).
 * https://oeis.org/A062980
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(0) = 1, a(1) = 5; for n > 1, a(n) = 6*n*a(n-1) + Σ_{k=1}^{n-2}
 *            a(k)*a(n-k-1) (offset 0)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 22,
 *            big any n
 * Helpers:   seq_alloc_new_array, seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_alloc_new_array.h"
#include "../utils/seq_free_array.h"

seq_t a(idx_t n) {
    /* Static storage for memoization */
    static seq_t *arr = NULL;
    static size_t alloc_len = 0;   /* allocated capacity (size of arr)   */
    static idx_t last = 1;         /* highest index actually initialized */

    /* Initial allocation and base cases */
    if (arr == NULL) {
        alloc_len = 2;
        arr = seq_alloc_new_array(alloc_len);
        arr[0] = (seq_t)1;
        arr[1] = (seq_t)5;
    }

    /* Grow array if needed */
    if ((size_t)n + 1 > alloc_len) {
        size_t new_len = (size_t)n + 1;
        seq_t *tmp = seq_alloc_new_array(new_len);
        for (size_t i = 0; i < alloc_len; ++i) tmp[i] = arr[i];
        seq_free_array(arr);
        arr = tmp;
        alloc_len = new_len;
    }

    /* Compute missing values */
    if (n > last) {
        for (idx_t m = last + 1; m <= n; ++m) {
            seq_t sum = (seq_t)0;
            if (m >= 2) {                /* Σ_{k=1}^{m-2} a(k)*a(m-k-1) */
                for (idx_t k = 1; k <= m - 2; ++k) {
                    sum += arr[k] * arr[m - k - 1];
                }
            }
            seq_t term1 = (seq_t)6 * (seq_t)m * arr[m - 1];
            arr[m] = term1 + sum;
        }
        last = n;
    }

    return arr[n];
}
