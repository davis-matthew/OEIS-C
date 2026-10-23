/*
 * A144533  Numerators of continued fraction convergents to sqrt(8/9).
 * https://oeis.org/A144533
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(0)=0, a(1)=1, a(n) = 16*a(n-1)+a(n-2) if n even, * otherwise
 *            a(n) = 2*a(n-1)+a(n-2), for n ≥ 2
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 13, u64 n <= 25, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of computed terms */
    static seq_t *buf = NULL;
    static size_t len = 0;   /* number of terms stored (always >= len) */
    static size_t cap = 0;   /* allocated capacity */

    /* handle trivial indices */
    if (n == 0) return 0;
    if (n == 1) return 1;

    /* ensure cache contains at least a(0) and a(1) */
    if (buf == NULL) {
        cap = 2;
        buf = (seq_t*)malloc(cap * sizeof(seq_t));
        if (buf == NULL) exit(1);          /* allocation failure */
        buf[0] = 0;
        buf[1] = 1;
        len = 2;
    }

    /* extend cache if needed */
    if ((size_t)n >= len) {
        size_t needed = (size_t)n + 1;
        if (needed > cap) {
            seq_t *tmp = (seq_t*)realloc(buf, needed * sizeof(seq_t));
            if (tmp == NULL) exit(1);      /* allocation failure */
            buf = tmp;
            cap = needed;
        }

        /* compute missing terms */
        for (size_t i = len; i <= (size_t)n; ++i) {
            if ((i & 1) == 0)   /* i is even */
                buf[i] = (seq_t)16 * buf[i - 1] + buf[i - 2];
            else                /* i is odd */
                buf[i] = (seq_t)2  * buf[i - 1] + buf[i - 2];
        }
        len = (size_t)n + 1;
    }

    return buf[(size_t)n];
}
