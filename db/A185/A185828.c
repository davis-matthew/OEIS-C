/*
 * A185828  Half the number of n X 2 binary arrays with every element equal to
 *          exactly one or two of its horizontal and vertical neighbors.
 * https://oeis.org/A185828
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) + a(n-2) + 2*a(n-3) - a(n-4) (n>4) a(1) = 1,
 *            a(2) = 3, a(3) = 10, a(4) = 23
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 23, u64 n <= 46, u128 n <= 92,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;      /* undefined below offset */

    /* return the initial four terms immediately */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)3;
    if (n == 3) return (seq_t)10;
    if (n == 4) return (seq_t)23;

    /* Allocate a temporary array big enough for the required terms.
       For n up to about 10 000 this is negligible. */
    size_t sz = (size_t)n;
    seq_t *buf = (seq_t *)malloc(sz * sizeof(seq_t));
    if (!buf) exit(1);        /* allocation failed */

    /* seed the first four values */
    buf[0] = (seq_t)1;
    buf[1] = (seq_t)3;
    buf[2] = (seq_t)10;
    buf[3] = (seq_t)23;

    /* compute successive terms using the recurrence */
    for (idx_t i = 4; i < n; ++i) {
        buf[i] = 2 * buf[i - 1] + buf[i - 2] + 2 * buf[i - 3] - buf[i - 4];
    }

    seq_t result = buf[n - 1];
    free(buf);
    return result;
}
