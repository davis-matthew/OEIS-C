/*
 * A170504  Number of reduced words of length n in Coxeter group on 15
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^46 = I.
 * https://oeis.org/A170504
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1; * For 1 ≤ n ≤ 45: a(n)=13·∑_{k=1}^{n} a(n−k) + 2 * For
 *            n=46: a(46)=13·∑_{k=1}^{45} a(46−k) – 91·a(0) + 1 * For n≥47:
 *            a(n)=13·∑_{k=1}^{45} a(n−k) – 91·a(n−46) * (This recurrence
 *            follows from the generating function in OEIS A170504.)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    /* Allocate array to hold a[0..n] */
    seq_t *arr = (seq_t*)malloc((n + 1) * sizeof(seq_t));
    arr[0] = (seq_t)1;

    for (idx_t i = 1; i <= n; ++i) {
        seq_t sum = 0;
        idx_t kmax = (i <= 45 ? i : 45);
        for (idx_t k = 1; k <= kmax; ++k) sum += arr[i - k];

        if (i <= 45) {
            arr[i] = (seq_t)13 * sum + (seq_t)2;
        } else if (i == 46) {
            arr[i] = (seq_t)13 * sum - (seq_t)91 * arr[0] + (seq_t)1;
        } else { /* i >= 47 */
            arr[i] = (seq_t)13 * sum - (seq_t)91 * arr[i - 46];
        }
    }

    seq_t result = arr[n];
    free(arr);
    return result;
}
