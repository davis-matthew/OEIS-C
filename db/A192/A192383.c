/*
 * A192383  Constant term of the reduction by x^2 -> x+1 of the polynomial
 *          p(n,x) defined below in Comments.
 * https://oeis.org/A192383
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=1, a(2)=0, a(3)=6, a(4)=8,
 *            a(n)=2*a(n-1)+8*a(n-2)-4*a(n-3)-4*a(n-4) for n≥5
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 18, u64 n <= 35, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* values below the offset are irrelevant */
    if (n < 1) return 0;

    /* static cache of previously computed terms */
    static seq_t *buf = NULL;          /* allocated array of terms          */
    static size_t buf_size = 0;        /* number of terms already stored    */
    static size_t buf_cap  = 0;        /* current array capacity (in terms) */

    /* if the required term is already cached, return it immediately */
    if ((size_t)n <= buf_size) return buf[n - 1];

    /* enlarge the cache if necessary */
    if (buf_cap < (size_t)n) {
        size_t new_cap = buf_cap ? buf_cap * 2 : 1;
        if (new_cap < (size_t)n) new_cap = (size_t)n;

        /* allocate new storage */
#ifdef __cplusplus
        seq_t *new_buf = new seq_t[new_cap];
        /* copy previously computed terms */
        for (size_t i = 0; i < buf_size; ++i) new_buf[i] = buf[i];
        /* free the old storage */
        delete[] buf;
#else
        /* C: use malloc (no constructors are required for integer types) */
        seq_t *new_buf = (seq_t*)malloc(new_cap * sizeof(seq_t));
        memcpy(new_buf, buf, buf_size * sizeof(seq_t));
        free(buf);
#endif
        buf = new_buf;
        buf_cap = new_cap;
    }

    /* compute missing terms up to n */
    for (size_t i = buf_size; i < (size_t)n; ++i) {
        if (i == 0)            { buf[0] = 1;            continue; }
        if (i == 1)            { buf[1] = 0;            continue; }
        if (i == 2)            { buf[2] = 6;            continue; }
        if (i == 3)            { buf[3] = 8;            continue; }
        /* recurrence for i≥4 (i corresponds to a(i+1)) */
        seq_t v = 2 * buf[i - 1] + 8 * buf[i - 2];
        v -= 4 * buf[i - 3];
        v -= 4 * buf[i - 4];
        buf[i] = v;
    }

    buf_size = (size_t)n;
    return buf[n - 1];
}
