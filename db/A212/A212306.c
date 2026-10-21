/*
 * A212306  Starting with the positive numbers, each element subtracts its
 *          value, a(n), from the next a(n) elements.
 * https://oeis.org/A212306
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(n) = n - ∑{i<n | a(i)+i≥n} a(i) Computed by simulating the
 *            subtractive process on the natural numbers. a(1)=1, a(2)=1,
 *            a(3)=2, a(4)=2, a(5)=1, a(6)=3, … (OEIS A212306)
 * Range:     u8 n <= 266, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;        /* not used, offset starts at 1 */
    /* Allocate array 1 … n (index 0 unused) */
    seq_t *arr = (seq_t *)malloc((size_t)(n + 1) * sizeof(*arr));
    if (!arr) return (seq_t)0;          /* allocation failure */

    /* Initialise with the natural numbers */
    for (idx_t i = 1; i <= n; ++i) {
        arr[i] = (seq_t)i;
    }

    /* Simulate the subtraction process */
    for (idx_t i = 1; i <= n; ++i) {
        seq_t val = arr[i];
        if (val == (seq_t)0) continue;

        /* Determine how far the subtraction reaches, without overflow */
        idx_t limit;
        if (val > (seq_t)(n - i)) {
            limit = n;
        } else {
            limit = i + (idx_t)val;
        }

        for (idx_t j = i + 1; j <= limit; ++j) {
            arr[j] -= val;
        }
    }

    seq_t result = arr[n];
    free(arr);
    return result;
}
