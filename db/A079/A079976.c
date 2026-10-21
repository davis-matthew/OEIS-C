/*
 * A079976  Expansion of g.f. 1/(1-x-x^2-x^4-x^5).
 * https://oeis.org/A079976
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1)+a(n-2)+a(n-4)+a(n-5), with initial values a(0)=1,
 *            a(1)=1, a(2)=2, a(3)=3, a(4)=6, a(5)=11
 * Range:     u8 n <= 10, u16 n <= 19, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* handle the first six terms explicitly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;
    if (n == 3) return (seq_t)3;
    if (n == 4) return (seq_t)6;
    if (n == 5) return (seq_t)11;

    /* maintain a sliding window of the last six computed values */
    seq_t buf[6];
    buf[0] = (seq_t)1;  /* a(0) */
    buf[1] = (seq_t)1;  /* a(1) */
    buf[2] = (seq_t)2;  /* a(2) */
    buf[3] = (seq_t)3;  /* a(3) */
    buf[4] = (seq_t)6;  /* a(4) */
    buf[5] = (seq_t)11; /* a(5) */

    for (idx_t i = 6; i <= n; ++i) {
        /* compute a(i) = a(i-1)+a(i-2)+a(i-4)+a(i-5) */
        seq_t val = buf[(i - 1) % 6] + buf[(i - 2) % 6];
        val += buf[(i - 4) % 6] + buf[(i - 5) % 6];

        /* store a(i) in the buffer */
        buf[i % 6] = val;
    }

    return buf[n % 6];
}
