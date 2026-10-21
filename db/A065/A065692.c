/*
 * A065692  Braided power sequence: this is b(n+1) = 3*b(n) + 2*d(n) - c(n),
 *          A065693 is c(n+1) = 3*c(n) + 2*b(n) - d(n) and A065694 is d(n+1) =
 *          3*d(n) + 2*c(n) - b(n), starting with b(0) = 0, c(0) = 1 and d(0)
 *          = 2.
 * https://oeis.org/A065692
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 9*a(n-1) - 33*a(n-2) + 52*a(n-3), with initial terms a(0)
 *            = 0, a(1) = 3, a(2) = 24
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *buf = NULL;        /* cache for computed terms   */
    static idx_t buf_len = 0;        /* number of terms stored     */

    /* initialize cache with first three terms */
    if (buf_len == 0) {
        buf_len = 3;
        buf = (seq_t *)malloc(sizeof(seq_t) * buf_len);
        if (!buf) return 0;          /* failure fallback           */
        buf[0] = (seq_t)0;
        buf[1] = (seq_t)3;
        buf[2] = (seq_t)24;
    }

    /* if the requested term is already cached, return it */
    if (n < buf_len)
        return buf[n];

    /* extend the buffer to hold up to term n */
    if (n + 1 > buf_len) {
        idx_t new_len = n + 1;
        seq_t *tmp = (seq_t *)realloc(buf, sizeof(seq_t) * new_len);
        if (!tmp) return 0;          /* failure fallback           */
        buf = tmp;
        buf_len = new_len;
    }

    /* compute missing terms using the recurrence */
    for (idx_t i = 3; i <= n; ++i) {
        buf[i] = 9 * buf[i - 1] - 33 * buf[i - 2] + 52 * buf[i - 3];
    }

    return buf[n];
}
