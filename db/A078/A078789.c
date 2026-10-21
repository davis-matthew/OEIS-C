/*
 * A078789  Expansion of (1-4*x+2*x^2)/(1-7*x+13*x^2-4*x^3).
 * https://oeis.org/A078789
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7*a(n-1) - 13*a(n-2) + 4*a(n-3); Initial values: a(0)=1,
 *            a(1)=3, a(2)=10
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static buffer for memoised values */
    static seq_t *buf = NULL;
    static idx_t buf_len = 0;  /* capacity of buf */
    static idx_t computed = 0; /* number of entries already filled */

    /* Initialise the first three terms on first call */
    if (buf == NULL) {
        buf_len = 8;
        buf = (seq_t*)malloc(buf_len * sizeof(seq_t));
        if (!buf) return 0;               /* allocation failure */
        buf[0] = 1;
        buf[1] = 3;
        buf[2] = 10;
        computed = 3;
    }

    /* Grow the buffer if needed */
    while (computed <= n) {
        if (computed >= buf_len) {
            buf_len = (buf_len > 0) ? buf_len * 2 : 8;
            seq_t *tmp = (seq_t*)realloc(buf, buf_len * sizeof(seq_t));
            if (!tmp) return 0;           /* allocation failure */
            buf = tmp;
        }
        /* Compute a(computed) using the recurrence */
        seq_t t1 = 7 * buf[computed - 1];
        seq_t t2 = 4 * buf[computed - 3];
        seq_t sum = t1 + t2;
        seq_t sub = 13 * buf[computed - 2];
        buf[computed] = sum - sub;
        ++computed;
    }

    return buf[n];
}
