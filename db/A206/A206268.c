/*
 * A206268  Number of compositions of n with at most one 1.
 * https://oeis.org/A206268
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) + a(n-2) - 2*a(n-3) - a(n-4) for n ≥ 4 with
 *            initial values a(0)=1, a(1)=1, a(2)=1, a(3)=3
 * Range:     u8 n <= 11, u16 n <= 22, u32 n <= 44, u64 n <= 88, u128 n <= 179,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static buffer for memoised values */
    static seq_t *buf = NULL;
    static size_t cap = 0;      /* allocated capacity */
    static idx_t last = 0;      /* largest index that has been computed */

    /* Ensure we have room for the first 4 values */
    if (!buf) {
        cap = 4;
        buf = (seq_t*)malloc(cap * sizeof(seq_t));
        if (!buf) return 0;          /* allocation failed */
        buf[0] = 1; buf[1] = 1; buf[2] = 1; buf[3] = 3;
        last = 3;
    }

    if (n <= last) return buf[(size_t)n];

    /* Grow array if necessary */
    if (cap <= (size_t)n) {
        size_t newcap = cap ? cap * 2 : 4;
        while (newcap <= (size_t)n) newcap <<= 1;
        seq_t *tmp = (seq_t*)realloc(buf, newcap * sizeof(seq_t));
        if (!tmp) return 0;          /* allocation failed */
        buf = tmp;
        cap = newcap;
    }

    /* Compute new entries up to n */
    for (idx_t i = last + 1; i <= n; ++i) {
        /* recurrence: a(i) = 2*a(i-1) + a(i-2) - 2*a(i-3) - a(i-4) */
        seq_t t1 = 2 * buf[(size_t)(i - 1)];
        seq_t t2 = buf[(size_t)(i - 2)];
        seq_t t3 = 2 * buf[(size_t)(i - 3)];
        seq_t t4 = buf[(size_t)(i - 4)];
        buf[(size_t)i] = t1 + t2 - t3 - t4;
    }

    last = n;
    return buf[(size_t)n];
}
