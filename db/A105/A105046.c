/*
 * A105046  a(n) = 1298*a(n-3) - a(n-6) - 648, for n>6, with a(0)=0, a(1)=1,
 *          a(2)=8, a(3)=145, a(4)=505, a(5)=9728, a(6)=187561.
 * https://oeis.org/A105046
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1298*a(n-3) - a(n-6) - 648, n > 6 * Initial values: a(0)
 *            = 0, a(1) = 1, a(2) = 8, a(3) = 145, * a(4) = 505, a(5) = 9728,
 *            a(6) = 187561
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Compute the OEIS sequence A105046 */
seq_t a(idx_t n)
{
    /* We lazily cache computed terms.  The cache lives across
       calls so that subsequent requests are served in O(1). */
    static seq_t *buf = NULL;   /* array holding a(0), a(1), ... */
    static size_t sz  = 0;      /* number of elements currently stored */

    /* Handle the small indices directly */
    if (n < 7) {
        static const seq_t init[7] = {
            0,          /* a(0) */
            1,          /* a(1) */
            8,          /* a(2) */
            145,        /* a(3) */
            505,        /* a(4) */
            9728,       /* a(5) */
            187561      /* a(6) */
        };
        return init[(size_t)n];
    }

    /* Ensure the cache is at least large enough for initial 7 terms */
    if (sz < 7) {
        sz = 7;
        buf = (seq_t *)malloc(sz * sizeof(seq_t));
        if (!buf) abort();                  /* OOM */
        buf[0] = 0;
        buf[1] = 1;
        buf[2] = 8;
        buf[3] = 145;
        buf[4] = 505;
        buf[5] = 9728;
        buf[6] = 187561;
    }

    /* Extend the cache if we need a larger index */
    if ((size_t)n >= sz) {
        seq_t *tmp = (seq_t *)realloc(buf, ((size_t)n + 1) * sizeof(seq_t));
        if (!tmp) abort();                  /* OOM */
        buf = tmp;

        /* Compute missing terms sequentially */
        for (size_t i = sz; i <= (size_t)n; ++i) {
            /* i >= 7 here, so i-3 and i-6 are valid indices */
            buf[i] = (seq_t)1298 * buf[i - 3] - buf[i - 6] - (seq_t)648;
        }
        sz = (size_t)n + 1;
    }

    return buf[(size_t)n];
}
