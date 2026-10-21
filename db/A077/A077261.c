/*
 * A077261  Triangular numbers that are 5 times another triangular number.
 * https://oeis.org/A077261
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 322*a(n-2) - a(n-4) + 120, with a(0)=0, a(1)=15,
 *            a(2)=105, a(3)=4950
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 15, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *buf = NULL;
    static idx_t buf_cap = 0;
    static idx_t buf_len = 0;

    /* initialize storage with the first four values */
    if (buf_len == 0) {
        /* start with a modest capacity, grow as needed */
        buf_cap = 16;
        buf = (seq_t*)malloc(sizeof(seq_t)*buf_cap);

        /* the first four terms */
        buf[0] = (seq_t)0;
        buf[1] = (seq_t)15;
        buf[2] = (seq_t)105;
        buf[3] = (seq_t)4950;
        buf_len = 4;
    }

    /* grow buffer if we have not yet computed up to n */
    if (n >= buf_len) {
        if (n + 1 > buf_cap) {
            while (buf_cap <= n)
                buf_cap <<= 1;        /* double capacity until big enough */
            buf = (seq_t*)realloc(buf, sizeof(seq_t)*buf_cap);
        }
        /* compute terms sequentially up to n */
        for (idx_t i = buf_len; i <= n; ++i) {
            if (i < 4) continue;           /* already initialized */
            /* recurrence: a(i) = 322*a(i-2) - a(i-4) + 120 */
            seq_t val = 322 * buf[i-2] - buf[i-4] + 120;
            buf[i] = val;
        }
        buf_len = n + 1;
    }

    return buf[n];
}
