/*
 * A129639  Number of meaningful differential operations of the k-th order on
 *          the space R^12.
 * https://oeis.org/A129639
 *
 * Offset:    12    (a(12) is the first term)
 * Keywords:  nonn
 * Formula:   recurrence a(n) = 6*a(n-2)-10*a(n-4)+4*a(n-6) for n>=18 initial
 *            values: a(12)=12, a(13)=22, a(14)=40, a(15)=74, a(16)=136,
 *            a(17)=252
 * Range:     u8 n <= 17, u16 n <= 26, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 12)
        return 0;   /* undefined for indices below the offset */

    if (n <= 17) {   /* direct return for the first 6 terms */
        static const seq_t base[6] = {12, 22, 40, 74, 136, 252};
        return base[n - 12];
    }

    /* Allocate array of length (n-12+1) to store a(12)...a(n) */
    size_t len = (size_t)(n - 12 + 1);
    seq_t *arr = (seq_t *)malloc(len * sizeof(seq_t));
    if (!arr)
        return 0;   /* allocation failure, unlikely */

    /* Initialize the known terms */
    arr[0] = 12;   /* a(12) */
    arr[1] = 22;   /* a(13) */
    arr[2] = 40;   /* a(14) */
    arr[3] = 74;   /* a(15) */
    arr[4] = 136;  /* a(16) */
    arr[5] = 252;  /* a(17) */

    /* Compute subsequent terms using the recurrence */
    for (idx_t i = 18; i <= n; ++i) {
        idx_t idx = i - 12;                     /* index in arr */
        arr[idx] = 6 * arr[idx - 2] - 10 * arr[idx - 4] + 4 * arr[idx - 6];
    }

    seq_t result = arr[len - 1];
    free(arr);
    return result;
}
