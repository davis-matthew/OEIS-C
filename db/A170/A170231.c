/*
 * A170231  Number of reduced words of length n in Coxeter group on 30
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^40 = I.
 * https://oeis.org/A170231
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Sequence A170231: Number of reduced words of length n in the Coxeter
   group on 30 generators S_i with relations (S_i)^2 = (S_i S_j)^40 = I.
   The sequence satisfies the following recurrence (derived from the
   rational generating function given in OEIS):

   a(0) = 1
   for 1 <= n <= 39:   a(n) = 28 * sum_{k=0}^{n-1} a(k) + 2
   a(40)             = 28 * sum_{k=1}^{39} a(k) - 405            // 405 = 406*1 - 1
   for n >= 41:        a(n) = 28 * sum_{k=n-1}^{n-39} a(k) - 406 * a(n-40)

   This implementation uses an incremental approach and requires O(n)
   time and O(n) memory (n <= 10000).  The routine works for arbitrary
   precision seq_t (e.g. C++ unsigned __int128 or arbitrary‑precision
   integer class) and for standard C unsigned integer types.
*/

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* a[0] must always be 1 */
    if (n == 0U) {
        return (seq_t)1;
    }

    /* Allocate an array large enough to hold all terms up to n. */
    seq_t *arr = (seq_t *)malloc((n + 1ULL) * sizeof(seq_t));
    if (!arr) {
        /* In case malloc fails, return zero (should never happen). */
        return (seq_t)0;
    }

    /* Initial term */
    arr[0] = (seq_t)1;
    seq_t sum = arr[0];          /* sum of a[0..i-1] used for recurrence */

    /* Compute terms 1..39 */
    idx_t i;
    for (i = 1; i <= n && i <= 39ULL; ++i) {
        arr[i] = (seq_t)28 * sum + (seq_t)2;
        sum += arr[i];
    }
    if (i > n) {    /* n <= 39 */
        seq_t result = arr[n];
        free(arr);
        return result;
    }

    /* Compute term 40 */
    idx_t start = 1;   /* start index for sum_{k=1}^{39} */
    seq_t sum1_39 = sum - arr[0];   /* sum of a[1..39] */
    arr[40] = (seq_t)28 * sum1_39 - (seq_t)405;   /* a[40] = 28*sum1_39 - 405 */
    sum += arr[40];      /* update running sum if later needed */

    if (n == 40ULL) {
        seq_t result = arr[40];
        free(arr);
        return result;
    }

    /* Compute terms 41..n using the homogeneous recurrence */
    for (i = 41; i <= n; ++i) {
        seq_t sub_sum = 0;
        /* Sum a[i-1] .. a[i-39] (39 terms) */
        for (idx_t j = i - 1; j >= i - 39ULL; --j) {
            sub_sum += arr[j];
        }
        arr[i] = (seq_t)28 * sub_sum - (seq_t)406 * arr[i - 40ULL];
    }

    seq_t result = arr[n];
    free(arr);
    return result;
}
