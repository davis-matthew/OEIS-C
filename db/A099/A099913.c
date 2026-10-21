/*
 * A099913  Related to the Herschel graph.
 * https://oeis.org/A099913
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0)=1, a(1)=3, a(2)=25, a(n)=10*a(n‑1)+a(n‑2)‑10*a(n‑3) for n≥3
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Fast returns for the first three terms */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)3;
    if (n == 2) return (seq_t)25;

    /* Static cache for already computed values */
    static seq_t *buf    = NULL;
    static size_t cap     = 0;     /* allocated size          */
    static size_t max_n   = 0;     /* highest index computed  */
    static int    init    = 0;     /* cache initialisation flag */

    /* Initialise the cache on the first call */
    if (!init) {
        cap = 8;
        buf = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!buf) exit(1);
        buf[0] = (seq_t)1;
        buf[1] = (seq_t)3;
        buf[2] = (seq_t)25;
        max_n = 2;
        init  = 1;
    }

    /* Grow the cache if necessary */
    if ((size_t)n > max_n) {
        size_t needed = (size_t)n + 1;
        if (cap < needed) {
            size_t newcap = cap;
            while (newcap < needed) newcap <<= 1;
            seq_t *tmp = (seq_t *)realloc(buf, newcap * sizeof(seq_t));
            if (!tmp) exit(1);
            buf = tmp;
            cap = newcap;
        }

        /* Compute all terms up to n (recurrence is linear) */
        for (size_t i = max_n + 1; i <= (size_t)n; ++i) {
            /* Use seq_t arithmetic; for integer seq_t types this
               may overflow as usual, which is acceptable because
               the caller may use a larger type if needed. */
            buf[i] = buf[i-1] * (seq_t)10 + buf[i-2]
                     - buf[i-3] * (seq_t)10;
        }
        max_n = (size_t)n;
    }

    return buf[(size_t)n];
}
