/*
 * A099529  Expansion of (1+x)^2/((1+x)^2+x^3).
 * https://oeis.org/A099529
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   not stated
 * Range:     i8 n <= 11, i16 n <= 21, i32 n <= 40, i64 n <= 40, i128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = -2*a(n-1) - a(n-2) - a(n-3);  a(0)=1, a(1)=0, a(2)=0. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *buf = NULL;        /* memoized values */
    static idx_t last = (idx_t)-1;   /* highest index stored */
    static size_t cap = 0;           /* current allocation size */

    /* first call: set up initial terms */
    if (last == (idx_t)-1) {
        cap = 8;
        buf = (seq_t*)malloc(cap * sizeof(seq_t));
        if (!buf) return (seq_t)0;   /* allocation failure */
        buf[0] = (seq_t)1;
        buf[1] = (seq_t)0;
        buf[2] = (seq_t)0;
        last = 2;
    }

    /* grow storage if necessary */
    if (n > (idx_t)cap - 1) {
        size_t newcap = cap;
        while (newcap <= (size_t)n) newcap <<= 1;
        seq_t *tmp = (seq_t*)realloc(buf, newcap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;   /* realloc failure */
        buf = tmp;
        cap = newcap;
    }

    /* compute terms up to n */
    while (last < n) {
        ++last;
        seq_t v = (seq_t)2 * buf[last-1];   /* 2*a(n-1) */
        v = v + buf[last-2] + buf[last-3];  /* +a(n-2)+a(n-3) */
        buf[last] = -v;                     /* negate to obtain the recurrence */
    }

    return buf[n];
}
