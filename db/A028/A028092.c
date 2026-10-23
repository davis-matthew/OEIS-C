/*
 * A028092  Expansion of 1/((1-3x)(1-7x)(1-8x)(1-12x)).
 * https://oeis.org/A028092
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (12^(n+3)-9*8^(n+3)+9*7^(n+3)-3^(n+3))/180
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence derived from 1/((1-3x)(1-7x)(1-8x)(1-12x)):
   a(0)=1, a(1)=30, a(2)=583, a(3)=9360,
   a(n)=30*a(n-1)-317*a(n-2)+1380*a(n-3)-2016*a(n-4)  (n>=4) */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *buf = NULL;
    static size_t len = 0;   /* number of computed terms (buf[0..len-1]) */
    static size_t cap = 0;   /* allocated capacity */

    /* Initialise on first call */
    if (buf == NULL) {
        cap = 16;                          /* initial capacity */
        buf = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!buf) return (seq_t)0;         /* unlikely in tests */
        buf[0] = (seq_t)1;
        buf[1] = (seq_t)30;
        buf[2] = (seq_t)583;
        buf[3] = (seq_t)9360;
        len = 4;
    }

    /* Grow and compute up to n */
    if ((size_t)n >= len) {
        size_t need = (size_t)n + 1;
        if (need > cap) {
            size_t newcap = cap * 2;
            if (newcap < need) newcap = need;
            seq_t *tmp = (seq_t *)realloc(buf, newcap * sizeof(seq_t));
            if (!tmp) { free(buf); return (seq_t)0; }
            buf = tmp;
            cap = newcap;
        }
        /* Compute missing terms */
        for (size_t i = len; i <= (size_t)n; ++i) {
            seq_t term;
            if (i == 0) term = (seq_t)1;
            else if (i == 1) term = (seq_t)30;
            else if (i == 2) term = (seq_t)583;
            else if (i == 3) term = (seq_t)9360;
            else {
                /* recurrence: a(n)=30*a(n-1)-317*a(n-2)+1380*a(n-3)-2016*a(n-4) */
                term = 30 * buf[i-1] - 317 * buf[i-2] + 1380 * buf[i-3] - 2016 * buf[i-4];
            }
            buf[i] = term;
        }
        len = (size_t)n + 1;
    }

    return buf[n];
}
