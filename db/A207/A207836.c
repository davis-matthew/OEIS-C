/*
 * A207836  a(n) = n*A052530(n)/2.
 * https://oeis.org/A207836
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   a(n) = n*A052530(n)/2 Recurrence (for n>5): a(n) = 8*a(n-1) -
 *            18*a(n-2) + 8*a(n-3) - a(n-4) Initial values (offset 2): a(2)=0,
 *            a(3)=3, a(4)=16, a(5)=75
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 33, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 2)
        return 0;                     /* offset is 2 */

    /* initial values */
    if (n <= 5) {
        static const seq_t init[6] = {0, 0, 0, 3, 16, 75};
        return init[n];
    }

    /* allocate workspace for recurrence */
    idx_t size = n + 1;
    seq_t *buf;
#ifdef __cplusplus
    buf = reinterpret_cast<seq_t*>(malloc(size * sizeof(seq_t)));
#else
    buf = (seq_t*)malloc(size * sizeof(seq_t));
#endif
    if (!buf)
        return 0;                     /* allocation failure – unlikely */

    /* set initial terms */
    buf[2] = 0;
    buf[3] = 3;
    buf[4] = 16;
    buf[5] = 75;

    /* compute all needed terms */
    for (idx_t i = 6; i <= n; ++i) {
        buf[i] = buf[i-1] * 8 - buf[i-2] * 18 + buf[i-3] * 8 - buf[i-4];
    }

    seq_t result = buf[n];
    free(buf);
    return result;
}
