/*
 * A071100  Expansion of (5 + 3*x + x^2 - x^3) / (1 - 2*x - 2*x^2 - 2*x^3 +
 *          x^4) in powers of x.
 * https://oeis.org/A071100
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) + 2*a(n-2) + 2*a(n-3) - a(n-4) with initial
 *            values a(0)=5, a(1)=13, a(2)=37, a(3)=109. This implements OEIS
 *            sequence A071100
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 19, u64 n <= 40, u128 n <= 82,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *buf = NULL;   /* cache of computed values */
    static idx_t sz  = 0;       /* number of entries cached (buf[0..sz-1]) */

    /* Handle small n directly with the initial values */
    if (n == 0) return (seq_t)5;
    if (n == 1) return (seq_t)13;
    if (n == 2) return (seq_t)37;
    if (n == 3) return (seq_t)109;

    /* Ensure the cache is initialized with the first four terms */
    if (buf == NULL) {
        sz = 4;
        buf = (seq_t *)malloc(sz * sizeof(seq_t));
        if (buf == NULL) return (seq_t)0;          /* out of memory, unlikely */
        buf[0] = (seq_t)5;
        buf[1] = (seq_t)13;
        buf[2] = (seq_t)37;
        buf[3] = (seq_t)109;
    }

    /* Extend the cache if we need to compute a larger index */
    if ((idx_t)(n + 1) > sz) {
        idx_t new_sz = n + 1;
        seq_t *tmp = (seq_t *)realloc(buf, new_sz * sizeof(seq_t));
        if (tmp == NULL) return (seq_t)0;          /* out of memory, unlikely */
        buf = tmp;
        for (idx_t i = sz; i <= n; ++i) {
            /* recurrence: a(i) = 2*a(i-1) + 2*a(i-2) + 2*a(i-3) - a(i-4) */
            seq_t t1 = 2 * buf[i - 1];
            seq_t t2 = 2 * buf[i - 2];
            seq_t t3 = 2 * buf[i - 3];
            buf[i] = t1 + t2 + t3 - buf[i - 4];
        }
        sz = new_sz;
    }

    return buf[n];
}
