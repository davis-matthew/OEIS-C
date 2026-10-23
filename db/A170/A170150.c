/*
 * A170150  Number of reduced words of length n in Coxeter group on 45
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^38 = I.
 * https://oeis.org/A170150
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1; for 1 ≤ n ≤ 37: a(n) = 2 + 43 *
 *            (a(n-1)+a(n-2)+...+a(n-37)); for n ≥ 38: a(n) = 43 *
 *            (a(n-1)+a(n-2)+...+a(n-37)) - 946 * a(n-38);
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

#ifdef __cplusplus
#include <vector>
#endif

seq_t a(idx_t n)
{
    /* Handle the trivial case */
    if (n == 0) {
        return (seq_t)1;
    }

#ifdef __cplusplus
    std::vector<seq_t> a_vec(n + 1);
    seq_t *arr = &a_vec[0];
#else
    seq_t *arr = (seq_t *)malloc((n + 1) * sizeof(seq_t));
#endif

    /* a(0) */
    arr[0] = (seq_t)1;

    /* Compute a(n) for 1 ≤ n ≤ 37 */
    if (n >= 1) {
        for (idx_t i = 1; i <= n && i <= 37; ++i) {
            seq_t sum = (seq_t)0;
            for (idx_t k = 1; k <= i; ++k) {
                sum += arr[i - k];
            }
            arr[i] = (seq_t)2 + (seq_t)43 * sum;
        }
    }

    /* If n ≤ 37 we are done */
    if (n <= 37) {
        seq_t result = arr[n];
#ifndef __cplusplus
        free(arr);
#endif
        return result;
    }

    /* Compute a(38) … a(n) using the recurrence of order 38 */
    seq_t sum37 = (seq_t)0;
    for (idx_t i = 1; i <= 37; ++i) {
        sum37 += arr[i];
    }

    for (idx_t i = 38; i <= n; ++i) {
        seq_t term = (seq_t)43 * sum37 - (seq_t)946 * arr[i - 38];
        arr[i] = term;
        /* Update the sliding sum of the last 37 terms */
        sum37 = sum37 + term - arr[i - 37];
    }

    seq_t result = arr[n];
#ifndef __cplusplus
    free(arr);
#endif
    return result;
}
