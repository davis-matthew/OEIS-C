/*
 * A077852  Expansion of (1-x)^(-1)/(1-2*x-x^3).
 * https://oeis.org/A077852
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) + a(n-3) + 1 , a(0)=1, a(1)=3, a(2)=7, a(3)=16
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 27, u64 n <= 55, u128 n <= 111,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Handle the first four terms directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)3;
    if (n == 2) return (seq_t)7;
    if (n == 3) return (seq_t)16;

    /* Allocate a temporary array large enough for the requested index.
       For the typical test ranges (n ≲ 10000) this is trivial in size. */
    idx_t i;
    seq_t *buf = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
    if (!buf) abort();           /* allocation failure – terminate */

    /* Initialise the first four values */
    buf[0] = (seq_t)1;
    buf[1] = (seq_t)3;
    buf[2] = (seq_t)7;
    buf[3] = (seq_t)16;

    /* Compute the remaining terms using the recurrence */
    for (i = 4; i <= n; ++i) {
        buf[i] = 2 * buf[i - 1] + buf[i - 3] + (seq_t)1;
    }

    seq_t result = buf[n];
    free(buf);
    return result;
}
