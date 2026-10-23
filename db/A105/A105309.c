/*
 * A105309  a(n) = |b(n)|^2 = x^2 + 3*y^2 where (x,y,y,y) is the quaternion
 *          b(n) of the sequence b of quaternions defined by b(0)=1,b(1)=1,
 *          b(n) = b(n-1) + b(n-2)*(0,c,c,c) where c = 1/sqrt(3).
 * https://oeis.org/A105309
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + 2*a(n-2) + a(n-3) - a(n-4) with initial values
 *            a(0)=1, a(1)=1, a(2)=2, a(3)=5
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Handle initial terms directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;
    if (n == 3) return (seq_t)5;

    /* Memoization buffer */
    static seq_t *buf = NULL;
    static idx_t buf_size = 0;   /* number of elements stored */

    /* Initialise buffer with the first four terms */
    if (buf_size == 0) {
        buf_size = 4;
        buf = (seq_t *)malloc(buf_size * sizeof(seq_t));
        if (!buf) return (seq_t)0;  /* allocation failed – return 0 */
        buf[0] = (seq_t)1;
        buf[1] = (seq_t)1;
        buf[2] = (seq_t)2;
        buf[3] = (seq_t)5;
    }

    /* If the requested index is already cached, just return it */
    if (n < buf_size)
        return buf[n];

    /* Otherwise, extend the buffer and compute new terms */
    idx_t old_size = buf_size;
    buf_size = n + 1;
    seq_t *tmp = (seq_t *)realloc(buf, buf_size * sizeof(seq_t));
    if (!tmp) return (seq_t)0;  /* allocation failed – return 0 */
    buf = tmp;

    for (idx_t i = old_size; i <= n; ++i) {
        seq_t val = buf[i - 1] + (seq_t)2 * buf[i - 2] + buf[i - 3] - buf[i - 4];
        buf[i] = val;
    }

    return buf[n];
}
