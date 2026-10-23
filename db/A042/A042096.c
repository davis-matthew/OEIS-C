/*
 * A042096  Numerators of continued fraction convergents to sqrt(572).
 * https://oeis.org/A042096
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy, less
 * Formula:   a(n) = 574*a(n-4) - a(n-8); a(0) = 23, a(1) = 24, a(2) = 263,
 *            a(3) = 287, a(4) = 13465, a(5) = 13752, a(6) = 150985, a(7) =
 *            164737
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 11, u64 n <= 27, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Base values for indices 0–7 */
    static const seq_t init[8] = {
        (seq_t)23, (seq_t)24, (seq_t)263, (seq_t)287,
        (seq_t)13465, (seq_t)13752, (seq_t)150985, (seq_t)164737
    };

    if (n < 8)
        return init[n];

    /* Allocate an array to hold all computed terms up to n */
    seq_t *arr = (seq_t*)malloc((size_t)(n + 1) * sizeof(seq_t));
    if (arr == NULL) /* allocation failed – return 0 as a fallback */
        return (seq_t)0;

    /* Initialize the first eight terms */
    for (idx_t i = 0; i < 8; ++i)
        arr[i] = init[i];

    /* Compute the remaining terms using the recurrence
       a(i) = 574*a(i-4) - a(i-8) for i >= 8 */
    for (idx_t i = 8; i <= n; ++i)
        arr[i] = (seq_t)574 * arr[i - 4] - arr[i - 8];

    seq_t result = arr[n];
    free(arr);
    return result;
}
