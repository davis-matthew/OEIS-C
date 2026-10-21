/*
 * A201082  Number of -2..2 arrays of n elements with first and second
 *          differences also in -2..2.
 * https://oeis.org/A201082
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   * a(n) = 3*a(n-1) + a(n-2) - 2*a(n-3) - 2*a(n-5) - 4*a(n-6) +
 *            2*a(n-8) + a(n-9) * a(1)=5, a(2)=19, a(3)=57, a(4)=175,
 *            a(5)=537, a(6)=1653, a(7)=5089, * a(8)=15663, a(9)=48207 *
 *            Offset: 1
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 38, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* Handle indices below offset – the specification says they are irrelevant. */
    if (n < 1)
        return (seq_t)0;

    /* Initial 9 values – a(1) .. a(9) */
    static const seq_t init[9] = {
        5, 19, 57, 175, 537, 1653, 5089, 15663, 48207
    };

    if (n <= 9u)
        return init[n - 1];

    /* Allocate an array of size n+1 (index 0 unused). */
    size_t sz = (size_t)n + 1;
    seq_t *vals = (seq_t *)malloc(sz * sizeof(seq_t));
    if (!vals)
        return (seq_t)0;  /* Out of memory – caller will not need the value. */

    /* Initialise first 9 values. */
    for (idx_t i = 1; i <= 9; ++i)
        vals[i] = init[i - 1];

    /* Compute the rest using the recurrence. */
    for (idx_t i = 10; i <= n; ++i) {
        vals[i] = 3 * vals[i - 1]
                 + vals[i - 2]
                 - 2 * vals[i - 3]
                 - 2 * vals[i - 5]
                 - 4 * vals[i - 6]
                 + 2 * vals[i - 8]
                 + vals[i - 9];
    }

    seq_t result = vals[n];
    free(vals);
    return result;
}
