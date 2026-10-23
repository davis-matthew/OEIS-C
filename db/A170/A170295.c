/*
 * A170295  Number of reduced words of length n in Coxeter group on 46
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^41 = I.
 * https://oeis.org/A170295
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: 
 *   a(0) = 1
 *   for 1 ≤ n ≤ 40:   a(n) = 2 + 44 * Σ_{i=0}^{n-1} a(i)
 *   a(41)              = 44 * Σ_{i=1}^{40} a(i) - 989
 *   for n ≥ 42:        a(n) = 44 * Σ_{i=n-40}^{n-1} a(i) - 990 * a(n-41)
 *
 *  The recurrence is derived from the rational generating function:
 *      (t^41 + 2 t^40 + … + 2 t + 1)/(1 - 44 t - … - 44 t^40 + 990 t^41)
 *
 *  The sequence is memoised; subsequent calls may compute additional
 *  terms only once.
 *
 *  The function a(n) returns the nth term (offset 0) of OEIS A170295.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static state for memoisation */
    static seq_t *arr = NULL;          /* array of computed terms */
    static size_t capacity = 0;        /* capacity of arr in elements */
    static idx_t last_computed = 0;    /* number of elements already computed */
    static seq_t sum_all = 0;          /* Σ_{i=0}^{last_computed-1} a(i)   */
    static seq_t sum_last40 = 0;       /* Σ_{i=(idx-40)}^{(idx-1)} a(i)   */

    /* initialise on first call */
    if (arr == NULL) {
        capacity = 1;
        arr = (seq_t*)malloc(capacity * sizeof(seq_t));
        arr[0] = 1;          /* a(0) = 1 */
        sum_all = 1;
        last_computed = 1;   /* next index to compute will be 1 */
    }

    /* extend storage if necessary */
    if (n + 1 > capacity) {
        capacity = n + 1;
        arr = (seq_t*)realloc(arr, capacity * sizeof(seq_t));
    }

    /* compute terms up to requested n */
    while (last_computed <= n) {
        idx_t idx = last_computed;   /* index to compute */

        if (idx == 0) { /* already initialised */
            /* nothing to do */
        } else if (idx <= 40) {
            /* a(n) = 2 + 44 * Σ_{i=0}^{n-1} a(i)   */
            arr[idx] = 2 + 44 * sum_all;
            sum_all += arr[idx];
        } else if (idx == 41) {
            /* a(41) = 44 * Σ_{i=1}^{40} a(i) - 989 */
            arr[idx] = 44 * (sum_all - arr[0]) - 989;
            sum_all += arr[idx];
        } else { /* idx >= 42 */
            if (idx == 42) {
                /* initialise window sum to Σ_{i=2}^{41} a(i) */
                sum_last40 = sum_all - arr[0] - arr[1];
            }
            /* a(n) = 44 * Σ_{i=n-40}^{n-1} a(i) - 990 * a(n-41) */
            arr[idx] = 44 * sum_last40 - 990 * arr[idx - 41];
            sum_all += arr[idx];
            /* slide window: remove a[idx-40], add a[idx] */
            sum_last40 += arr[idx] - arr[idx - 40];
        }

        last_computed++;
    }

    return arr[n];
}
