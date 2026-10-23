/*
 * A193519  a(n) = (2/3)*Sum_{i=1..n-1} A000129(i)*3^(n-i).
 * https://oeis.org/A193519
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 40, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 5*a(n-1) - 5*a(n-2) - 3*a(n-3), a(0)=0, a(1)=0, a(2)=2 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *buf = NULL;
    static idx_t sz = 0;

    if (buf == NULL) {
        buf = (seq_t *)malloc(3 * sizeof(seq_t));
        if (buf == NULL) return 0;             /* allocation failed */
        buf[0] = 0;
        buf[1] = 0;
        buf[2] = (seq_t)2;
        sz = 3;
    }

    if (n < sz)
        return buf[n];

    idx_t oldsz = sz;
    sz = n + 1;
    seq_t *newbuf = (seq_t *)realloc(buf, sz * sizeof(seq_t));
    if (newbuf == NULL)
        return 0;                               /* allocation failed */
    buf = newbuf;

    for (idx_t i = oldsz; i <= n; ++i) {
        if (i == 0 || i == 1) {
            buf[i] = 0;
        } else if (i == 2) {
            buf[i] = (seq_t)2;
        } else {
            buf[i] = 5 * buf[i - 1] - 5 * buf[i - 2] - 3 * buf[i - 3];
        }
    }

    return buf[n];
}
