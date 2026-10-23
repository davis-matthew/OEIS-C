/*
 * A026430  a(n) is the sum of first n terms of A001285 (Thue-Morse sequence).
 * https://oeis.org/A026430
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(0)=0, a(1)=1, a(2n)=3*n, a(2n+1)=-a(n)+a(n+1)+3*n
 * Range:     u8 n <= 170, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;

    /* Allocate workspace for a(0)…a(n) */
    seq_t *arr = (seq_t *)malloc(sizeof(seq_t) * ((size_t)n + 1));
    if (!arr) return (seq_t)0;              /* allocation failure */

    arr[0] = (seq_t)0;
    arr[1] = (seq_t)1;

    for (idx_t k = 2; k <= n; ++k)
    {
        if ((k & (idx_t)1) == 0)          /* k = 2m */
        {
            idx_t m = k >> 1;
            arr[k] = (seq_t)3 * (seq_t)m;
        }
        else                              /* k = 2m+1 */
        {
            idx_t m = (k - 1) >> 1;
            seq_t three_m = (seq_t)3 * (seq_t)m;
            arr[k] = arr[m + 1] + three_m - arr[m];
        }
    }

    seq_t result = arr[n];
    free(arr);
    return result;
}
