/*
 * A131965  a(n) = 1 + Sum_{i=2..n-1} n*a(i).
 * https://oeis.org/A131965
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + Sum_{i=2..n-1} n*a(i) (offset 0)
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *arr = NULL;          /* cache of a(i) */
    static size_t cap = 0;             /* allocated capacity */
    static idx_t max_computed = 0;     /* highest n computed */
    static seq_t sum_seen = (seq_t)0;  /* Σ_{i=2}^{max_computed} a(i) */

    /* Base cases */
    if (n <= 2)
        return (seq_t)1;

    /* Initial allocation and base values */
    if (arr == NULL) {
        cap = 3;
        arr = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!arr) return (seq_t)0;      /* allocation failed, return 0 */
        arr[0] = arr[1] = arr[2] = (seq_t)1;
        max_computed = 2;
        sum_seen = arr[2];              /* Σ_{i=2}^{2} a(i) = a(2) */
    }

    /* Grow array if necessary */
    if (n + 1 > cap) {
        size_t new_cap = cap;
        while (n + 1 > new_cap)
            new_cap *= 2;
        arr = (seq_t *)realloc(arr, new_cap * sizeof(seq_t));
        if (!arr) return (seq_t)0;
        cap = new_cap;
    }

    /* Compute any missing terms up to n */
    if (n > max_computed) {
        for (idx_t i = max_computed + 1; i <= n; ++i) {
            seq_t val = (seq_t)1 + (seq_t)i * sum_seen;
            arr[i] = val;
            sum_seen += val;          /* update running sum for next step */
        }
        max_computed = n;
    }

    return arr[n];
}
