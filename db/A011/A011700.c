/*
 * A011700  A binary m-sequence: expansion of reciprocal of x^8+x^7+x^3+x+1.
 * https://oeis.org/A011700
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0..6)=0, a(7)=1; For n>=8, a(n) = a(n-1) ^ a(n-3) ^ a(n-7) ^
 *            a(n-8) (binary recurrence)
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *buf = NULL;
    static idx_t len = 0;   /* number of computed elements already stored */

    /* initialize with the first 8 terms */
    if (len == 0) {
        buf = (seq_t *)malloc(8 * sizeof(seq_t));
        buf[0] = 0; buf[1] = 0; buf[2] = 0; buf[3] = 0;
        buf[4] = 0; buf[5] = 0; buf[6] = 0; buf[7] = 1;
        len = 8;
    }

    /* if the requested index is beyond what we have, extend the buffer */
    if (n >= len) {
        /* grow buffer to hold indices up to n */
        buf = (seq_t *)realloc(buf, (n + 1) * sizeof(seq_t));

        /* compute the missing terms using the recurrence */
        for (size_t i = (size_t)len; i <= (size_t)n; ++i) {
            /* i is guaranteed to be >= 8 here because len >= 8 */
            buf[i] = buf[i - 1] ^ buf[i - 3] ^ buf[i - 7] ^ buf[i - 8];
        }
        len = n + 1;
    }

    return buf[n];
}
