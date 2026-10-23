/*
 * A181626  Number of closed walks of length n in a kite graph (K4 with one
 *          edge deleted).
 * https://oeis.org/A181626
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 5*a(n-2) + 4*a(n-3) for n>4 with a(1)=4, a(2)=0, a(3)=10,
 *            a(4)=12
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 24, u64 n <= 48, u128 n <= 95,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    if (n == 1) return (seq_t)4;
    if (n == 2) return (seq_t)0;
    if (n == 3) return (seq_t)10;
    if (n == 4) return (seq_t)12;

    size_t len = (size_t)n;
    seq_t *arr = (seq_t *)malloc(len * sizeof(seq_t));
    if (!arr) return (seq_t)0; /* memory error fallback */

    arr[0] = (seq_t)4;      /* a(1) */
    arr[1] = (seq_t)0;      /* a(2) */
    arr[2] = (seq_t)10;     /* a(3) */
    arr[3] = (seq_t)12;     /* a(4) */

    for (idx_t i = 5; i <= n; ++i) {
        arr[i - 1] = (seq_t)5 * arr[i - 3] + (seq_t)4 * arr[i - 4];
    }

    seq_t result = arr[n - 1];
    free(arr);
    return result;
}
