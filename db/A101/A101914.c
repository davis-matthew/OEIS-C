/*
 * A101914  G.f. satisfies: A(x) = 1/(1 + x*A(x^4)) and also the continued
 *          fraction: 1 + x*A(x^5) = [1; 1/x, 1/x^4, 1/x^16, 1/x^64, ...,
 *          1/x^(4^(n-1)), ...].
 * https://oeis.org/A101914
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = - Sum_{k=0..⌊(n-1)/4⌋} a(k) * a(n-4*k-1), a(0)=1
 * Range:     i8 n <= 30, i16 n <= 62, i32 n <= 62, i64 n <= 62, i128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes terms of OEIS A101914 for any idx_t n, with memoisation. */
#if defined(__cplusplus)
#include <cstdlib>
#include <cstring>
#else
#include <stdlib.h>
#include <string.h>
#endif

seq_t a(idx_t n) {
    /* static array for memoised values */
    static seq_t *buf = NULL;
    static idx_t buflen = 0;

    if (n >= buflen) {                     /* need to grow array */
        idx_t newlen = n + 1;                /* inclusive of n */
        seq_t *newbuf = (seq_t*)malloc(newlen * sizeof(seq_t));
        if (newbuf == NULL) return 0;       /* allocation failure unlikely */

        /* copy any existing values */
        if (buf != NULL) {
            memcpy(newbuf, buf, buflen * sizeof(seq_t));
        }

        /* initialise from the first new index */
        if (buflen == 0) {                  /* first run, set a(0) */
            newbuf[0] = (seq_t)1;
            buflen = 1;
        }

        /* compute new terms up to n */
        for (idx_t i = buflen; i < newlen; ++i) {
            seq_t sum = (seq_t)0;
            idx_t maxk = (i - 1) / 4;
            for (idx_t k = 0; k <= maxk; ++k) {
                sum += newbuf[k] * newbuf[i - 4 * k - 1];
            }
            newbuf[i] = -sum;
        }

        /* replace old buffer */
        if (buf != NULL) free(buf);
        buf = newbuf;
        buflen = newlen;
    }

    return buf[n];
}
