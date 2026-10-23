/*
 * A154920  Denominators of a ternary BBP-type formula for log(3).
 * https://oeis.org/A154920
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2-(-1)^n)*(n+1)*3^n Recurrence: a(n) = 18*a(n-2) -
 *            81*a(n-4), with a(0)=1, a(1)=18, a(2)=27, a(3)=324
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 16, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* handle small indices directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)18;
    if (n == 2) return (seq_t)27;
    if (n == 3) return (seq_t)324;

    /* allocate array of length n+1 */
    size_t len = (size_t)(n + 1);
    seq_t *arr = (seq_t *)malloc(len * sizeof(seq_t));
    if (!arr) return (seq_t)0; /* allocation failure, return 0 */

    /* initialise first four values */
    arr[0] = (seq_t)1;
    arr[1] = (seq_t)18;
    arr[2] = (seq_t)27;
    arr[3] = (seq_t)324;

    /* compute using recurrence */
    for (idx_t i = 4; i <= n; ++i) {
        seq_t term1 = (seq_t)18 * arr[i - 2];
        seq_t term2 = (seq_t)81 * arr[i - 4];
        arr[i] = term1 - term2;
    }

    seq_t result = arr[n];
    free(arr);
    return result;
}
