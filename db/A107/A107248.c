/*
 * A107248  a(n) = 4*a(n-2) - 4*a(n-4) + 25*a(n-6).
 * https://oeis.org/A107248
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(n) = 4*a(n-2) - 4*a(n-4) + 25*a(n-6) Initial values: a(0)=1,
 *            a(1)=1, a(2)=1, a(3)=37, a(4)=37, a(5)=169
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 28, u64 n <= 58, u128 n <= 118,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#ifdef __cplusplus
#include <new>
#endif

seq_t a(idx_t n)
{
    /* static state for memoisation */
    static seq_t *buf = NULL;
    static idx_t cap = 0;          /* current allocation size */
    static idx_t size = 0;         /* number of initialized terms */

    /* small table of base values required to start the recurrence   */
    static const seq_t base[6] = {1, 1, 1, 37, 37, 169};

    /* first call – allocate a small buffer and initialise base terms */
    if (size == 0) {
        idx_t newcap = 16;
        if (newcap < 6) newcap = 6;  /* at least enough for the base */
#ifdef __cplusplus
        buf = new seq_t[newcap];
#else
        buf = (seq_t *)malloc(newcap * sizeof(seq_t));
#endif
        if (!buf) abort();
        for (idx_t i = 0; i < 6; ++i) buf[i] = base[i];
        size = 6;
        cap  = newcap;
    }

    /* if the requested index is already computed, simply return it */
    if (n < size) return buf[n];

    /* expand the buffer if necessary (doubling strategy) */
    if (n + 1 > cap) {
        idx_t newcap = cap ? cap << 1 : 16;
        while (newcap <= n) newcap <<= 1;
#ifdef __cplusplus
        seq_t *newbuf = new seq_t[newcap];
        for (idx_t i = 0; i < size; ++i) newbuf[i] = buf[i];
        delete[] buf;
        buf = newbuf;
#else
        buf = (seq_t *)realloc(buf, newcap * sizeof(seq_t));
        if (!buf) abort();
#endif
        cap = newcap;
    }

    /* compute missing terms using the recurrence */
    for (idx_t i = size; i <= n; ++i) {
        buf[i] = 4 * buf[i-2] - 4 * buf[i-4] + 25 * buf[i-6];
    }
    size = n + 1;

    return buf[n];
}
