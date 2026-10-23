/*
 * A183683  Number of (n+1) X 5 binary arrays with every 2 X 2 subblock
 *          nonsingular.
 * https://oeis.org/A183683
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 13*a(n-2) + 16*a(n-3) * Variables: * a(1) = 48 * a(2) =
 *            208 * a(3) = 832 * * The sequence is defined for n ≥ 1. For n <
 *            4, the values * are returned directly. For n ≥ 4, we compute
 *            iteratively * up to n, storing the intermediate results in a
 *            temporary * array. This keeps the implementation...
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 13, u64 n <= 29, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) {
        /* offset is 1; a(0) is not defined, but return 0 for safety */
        return (seq_t)0;
    }

    /* Allocate an array of n+1 elements (index 0 unused). */
    size_t sz = (size_t)(n + 1);
    seq_t *arr = (seq_t*)calloc(sz, sizeof(seq_t));
    if (!arr)
        return (seq_t)0; /* memory error: return 0 as a sensible fallback */

    /* Initial values */
    arr[1] = (seq_t)48;
    if (n > 1) arr[2] = (seq_t)208;
    if (n > 2) arr[3] = (seq_t)832;

    /* Compute the recurrence for n ≥ 4 */
    if (n > 3) {
        for (idx_t i = 4; i <= n; ++i) {
            arr[i] = 13 * arr[i - 2] + 16 * arr[i - 3];
        }
    }

    seq_t result = arr[n];
    free(arr);
    return result;
}
