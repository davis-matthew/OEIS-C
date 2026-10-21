/*
 * A060188  A column and diagonal of A060187.
 * https://oeis.org/A060188
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*a(n-1) - 7*a(n-2) + 3*a(n-3), with a(2)=1, a(3)=6,
 *            a(4)=23. Offset = 2
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 21, u64 n <= 41, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache */
    static seq_t *buf = NULL;
    static idx_t sz   = 0;   /* number of elements stored (highest index + 1) */
    static idx_t cap  = 0;   /* capacity of buf */

    /* Offset: we treat indices 0..n. For n < 2, return 0 (undefined). */
    if (n < 2) return (seq_t)0;

    /* Ensure capacity for indices up to n */
    if (cap < n + 1) {
        idx_t newcap = cap ? cap << 1 : 8;
        if (newcap < n + 1) newcap = n + 1;
        seq_t *tmp = (seq_t *)realloc(buf, newcap * sizeof(seq_t));
        if (!tmp) {
            /* allocation failed – abort (no recovery) */
            abort();
        }
        buf = tmp;
        cap = newcap;
    }

    /* Initialise base values if not yet done */
    if (sz == 0) {
        buf[0] = (seq_t)0;   /* unused */
        buf[1] = (seq_t)0;   /* unused */
        buf[2] = (seq_t)1;
        buf[3] = (seq_t)6;
        buf[4] = (seq_t)23;
        sz = 5;              /* indices 0..4 are valid */
    }

    /* Compute missing terms up to n */
    if (n >= sz) {
        for (idx_t i = sz; i <= n; ++i) {
            if (i == 2) {
                buf[2] = (seq_t)1;
            } else if (i == 3) {
                buf[3] = (seq_t)6;
            } else if (i == 4) {
                buf[4] = (seq_t)23;
            } else {
                /* recurrence: a(i) = 5*a(i-1) - 7*a(i-2) + 3*a(i-3) */
                seq_t t = (seq_t)5 * buf[i-1];
                t = t - (seq_t)7 * buf[i-2];
                t = t + (seq_t)3 * buf[i-3];
                buf[i] = t;
            }
        }
        sz = n + 1;
    }

    return buf[n];
}
