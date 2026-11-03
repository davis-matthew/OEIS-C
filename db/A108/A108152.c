/*
 * A108152  a(n)= 3*a(n-1) +2*a(n-2) +a(n-3).
 * https://oeis.org/A108152
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) + 2*a(n-2) + a(n-3); Initial values: a(0)=1,
 *            a(1)=0, a(2)=2
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 18, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Validated: partial, pass, 26 terms through n = 25 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    /* Memoization buffer */
    static seq_t *buf        = NULL;
    static idx_t buf_len     = 0;
    static idx_t buf_cap     = 0;

    /* Handle the first three terms directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)2;

    /* Initialize the buffer on first call */
    if (buf_len == 0) {
        buf_cap = 3;
        buf = mem_new(seq_t, (size_t)buf_cap);
        buf[0] = (seq_t)1;
        buf[1] = (seq_t)0;
        buf[2] = (seq_t)2;
        buf_len = 3;
    }

    /* Extend the buffer up to n */
    while (buf_len <= n) {
        idx_t i = buf_len;
        seq_t val = (seq_t)3 * buf[i - 1] + (seq_t)2 * buf[i - 2] + buf[i - 3];

        if (buf_len == buf_cap) {
            buf_cap = buf_cap ? buf_cap * 2 : 4;
            buf = mem_resize(seq_t, buf, (size_t)buf_cap);
        }
        buf[buf_len++] = val;
    }

    return buf[n];
}
