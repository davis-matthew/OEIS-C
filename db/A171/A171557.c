/*
 * A171557  a(n) = 3^n*A168053(n).
 * https://oeis.org/A171557
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   not stated
 * Range:     i8 n <= 3, i16 n <= 7, i32 n <= 16, i64 n <= 22, i128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence for OEIS sequence A171557:
 * a(0)=1, a(1)=3, a(2)=-9, a(3)=-81
 * a(n)=3*a(n-1)+27*a(n-3)-81*a(n-4)   for n>=4
 * this implements a(n)=3^n*A168053(n)   */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for memoisation */
    static seq_t *buf = NULL;      /* array of values */
    static idx_t size = 0;         /* number of elements computed */
    static idx_t cap  = 0;         /* capacity of array */

    /* Initialise the cache on first call */
    if (buf == NULL) {
        cap  = 4;                   /* store the four initial values */
        size = 4;
        buf = (seq_t *)malloc(sizeof(seq_t) * cap);
        if (buf == NULL) return 0;  /* fall back if allocation fails */
        buf[0] =  1;
        buf[1] =  3;
        buf[2] = -9;
        buf[3] = -81;
    }

    /* Return if already computed */
    if (n < size) return buf[n];

    /* Ensure enough allocation space */
    if (n+1 > cap) {
        idx_t newcap = cap;
        while (n+1 > newcap) newcap <<= 1;          /* double until big enough */
        seq_t *tmp = (seq_t *)realloc(buf, sizeof(seq_t) * newcap);
        if (tmp == NULL) return 0;                  /* abort on failure */
        buf = tmp;
        cap = newcap;
    }

    /* Compute from the current end up to n */
    for (idx_t i = size; i <= n; ++i) {
        seq_t val = 3 * buf[i-1] + 27 * buf[i-3] - 81 * buf[i-4];
        buf[i] = val;
    }
    size = n + 1;          /* up to n inclusive now stored */

    return buf[n];
}
