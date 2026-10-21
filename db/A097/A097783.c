/*
 * A097783  Chebyshev polynomials S(n,11) + S(n-1,11) with Diophantine
 *          property.
 * https://oeis.org/A097783
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 11*a(n-1) – a(n-2) with a(0) = 1, a(1) = 12
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* OEIS A097783:  S(n,11)+S(n-1,11) = 11*a(n-1) – a(n-2)   */
seq_t a(idx_t n)
{
    /* static memoisation buffer */
    static seq_t *buf = NULL;      /* storage for computed terms    */
    static idx_t  buf_len = 0;     /* number of terms currently stored */

    /* initialise the buffer with the first two terms */
    if (buf == NULL)
    {
        buf_len = 2;
        buf = (seq_t *)malloc((size_t)buf_len * sizeof(seq_t));
        buf[0] = 1;
        buf[1] = 12;
    }

    /* extend the buffer on demand */
    while (buf_len <= n)
    {
        seq_t t = buf[buf_len - 1];   /* a(k-1)   */
        t *= 11;                      /* 11*a(k-1) */
        t -= buf[buf_len - 2];        /* – a(k-2) */
        buf = (seq_t *)realloc(buf,
                               (size_t)(buf_len + 1) * sizeof(seq_t));
        buf[buf_len] = t;              /* a(k)     */
        buf_len++;
    }

    return buf[n];
}
