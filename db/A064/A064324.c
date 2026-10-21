/*
 * A064324  a(n) = a(n-1) + floor(a(n-2)/2) with a(0)=1, a(1)=2.
 * https://oeis.org/A064324
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + floor(a(n-2)/2), a(0) = 1, a(1) = 2
 * Range:     u8 n <= 17, u16 n <= 35, u32 n <= 71, u64 n <= 142,
 *            u128 n <= 284, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *buf = NULL;   /* memoization buffer */
    static idx_t  sz  = 0;      /* number of elements currently stored */

    /* If the requested term is already known, return it. */
    if (n < sz)
        return buf[n];

    /* Grow the buffer to accommodate index n. */
    size_t new_sz = (size_t)(n + 1);         /* must be >= sz */

#ifdef __cplusplus
    /* In C++ use operator new to construct objects correctly. */
    seq_t *newbuf = new seq_t[new_sz];
    for (idx_t i = 0; i < sz; ++i)
        newbuf[i] = buf[i];                  /* copy existing values */
    delete[] buf;                            /* free old buffer */
    buf = newbuf;
#else
    /* In C use realloc (creates raw memory). */
    buf = (seq_t *)realloc(buf, new_sz * sizeof(seq_t));
    if (!buf)
        return 0;                            /* out of memory; unlikely in tests */
#endif

    /* Initialise the base cases if they have not been done yet. */
    if (sz == 0) {
        buf[0] = (seq_t)1;
        if (new_sz > 1)
            buf[1] = (seq_t)2;
        sz = (idx_t)((new_sz > 1) ? 2 : 1);
    } else if (sz == 1) {
        buf[1] = (seq_t)2;
        sz = 2;
    }

    /* Extend the sequence up to the requested index. */
    for (idx_t i = sz; i <= n; ++i) {
        buf[i] = buf[i - 1] + (buf[i - 2] / (seq_t)2);
    }

    sz = (idx_t)new_sz;
    return buf[n];
}
