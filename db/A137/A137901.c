/*
 * A137901  Limiting sequence when we start with positive integers (A000027)
 *          and at step n >= 1 add to the term at position n + a(n) the value
 *          1.
 * https://oeis.org/A137901
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A137901:
   a(n) = n + count of m < n with a(m) + m = n.
   Computed by starting with a(i)=i and for n≥1 adding 1 to a[n + a[n]]. */

#include <stdlib.h>
#include <assert.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* Allocate array 1..n (index 0 unused). */
    seq_t *arr = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    assert(arr);

    /* Initial values a(i) = i. */
    for (idx_t i = 1; i <= n; ++i)
        arr[i] = (seq_t)i;

    /* Perform the updates for 1 ≤ m ≤ n. */
    const seq_t n_big = (seq_t)n;
    for (idx_t m = 1; m <= n; ++m) {
        const seq_t target = (seq_t)m + arr[m];
        if (target <= n_big)
            arr[(idx_t)target] += (seq_t)1;
    }

    const seq_t result = arr[n];
    free(arr);
    return result;
}
