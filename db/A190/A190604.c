/*
 * A190604  Number of (n+2)X(n+2) symmetric binary matrices without the
 *          pattern 1 0 1 diagonally.
 * https://oeis.org/A190604
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Π_{k=1}^{n+2} f(k) where f(0)=1, f(1)=2, f(2)=4, f(3)=7
 *            and for k ≥ 4, f(k) = f(k-1) + f(k-2) + f(k-4). (f(k) counts
 *            binary strings of length k containing no contiguous substring
 *            101)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 9, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* size of the matrix = n+2 */
    idx_t m = n + 2;                /* number of diagonal lengths */
    if (m == 0) return 1;           /* safety, though n>=1 */

    /* allocate array for f[0..m] */
    seq_t *f = (seq_t *)malloc((m + 1) * sizeof(seq_t));
    if (!f) return 0;               /* allocation failed */

    f[0] = (seq_t)1;
    if (m >= 1) f[1] = (seq_t)2;
    if (m >= 2) f[2] = (seq_t)4;
    if (m >= 3) f[3] = (seq_t)7;

    for (idx_t i = 4; i <= m; ++i) {
        /* recurrence: f(i) = f(i-1) + f(i-2) + f(i-4) */
        f[i] = f[i - 1] + f[i - 2] + f[i - 4];
    }

    seq_t prod = (seq_t)1;
    for (idx_t i = 1; i <= m; ++i) {
        prod *= f[i];
    }

    free(f);
    return prod;
}
