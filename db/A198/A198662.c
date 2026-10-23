/*
 * A198662  Number of n X 1 0..4 arrays with values 0..4 introduced in row
 *          major order and each element equal to one or two horizontal and
 *          vertical neighbors.
 * https://oeis.org/A198662
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 17, u32 n <= 29, u64 n <= 53, u128 n <= 100,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Empirical recurrence: a(n) = 4*a(n-1) + a(n-2) - 17*a(n-3) + 6*a(n-4) + 21*a(n-5) - 6*a(n-6) - 8*a(n-7)
   initial values a(1)=0, a(2)=1, a(3)=1, a(4)=2, a(5)=3, a(6)=6, a(7)=11, a(8)=23 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* safety, not part of the offset */

    /* Allocate a temporary array to compute the terms */
#ifdef __cplusplus
    seq_t *arr = new seq_t[n + 1];
#else
    seq_t *arr = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!arr) exit(EXIT_FAILURE);
#endif

    /* Initial terms */
    if (n >= 1) arr[1] = (seq_t)0;
    if (n >= 2) arr[2] = (seq_t)1;
    if (n >= 3) arr[3] = (seq_t)1;
    if (n >= 4) arr[4] = (seq_t)2;
    if (n >= 5) arr[5] = (seq_t)3;
    if (n >= 6) arr[6] = (seq_t)6;
    if (n >= 7) arr[7] = (seq_t)11;
    if (n >= 8) arr[8] = (seq_t)23;

    /* Compute by recurrence for n > 8 */
    for (idx_t i = 9; i <= n; ++i) {
        seq_t t = 4 * arr[i - 1] + arr[i - 2];
        t -= 17 * arr[i - 3];
        t += 6  * arr[i - 4];
        t += 21 * arr[i - 5];
        t -= 6  * arr[i - 6];
        t -= 8  * arr[i - 7];
        arr[i] = t;
    }

    seq_t result = arr[n];
#ifdef __cplusplus
    delete[] arr;
#else
    free(arr);
#endif
    return result;
}
