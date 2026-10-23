/*
 * A103201  a(1) = 11, a(2) = 19, a(3) = 89, a(4) = 151; for n >= 5, a(n) =
 *          sqrt(a(n-4)^2 + 60*a(n-2)^2 + 4*a(n-2)*sqrt(210 + 15*a(n-4)^2)).
 * https://oeis.org/A103201
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 20, u64 n <= 42, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A103201: a(1)=11, a(2)=19, a(3)=89, a(4)=151.
   For n >= 5 the sequence satisfies the linear recurrence
        a(n) = 8*a(n-2) - a(n-4)   (all operations on seq_t).
   This follows from the generating function
        x*(11+19*x+x^2-x^3)/(1-8*x^2+x^4). */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static buffer to memoise computed terms.
       buf[0] holds a(1), buf[1] holds a(2), etc. */
    static seq_t *buf = NULL;
    static idx_t buflen = 0;   /* number of stored terms */

    /* Grow the buffer if necessary.  Preserve the old size to know
       where the new terms start. */
    idx_t oldbuflen = buflen;
    if (n > buflen)
    {
        seq_t *newbuf = (seq_t *)realloc(buf, n * sizeof(seq_t));
        if (!newbuf) /* memory allocation failed */
            return (seq_t)0;
        buf = newbuf;
        buflen = n;
    }

    /* Compute any missing terms up to index n. */
    for (idx_t i = oldbuflen; i < n; ++i)
    {
        idx_t idx = i + 1;   /* 1‑based OEIS index */

        if (idx == 1)
            buf[i] = (seq_t)11;
        else if (idx == 2)
            buf[i] = (seq_t)19;
        else if (idx == 3)
            buf[i] = (seq_t)89;
        else if (idx == 4)
            buf[i] = (seq_t)151;
        else
        {
            seq_t a_nminus2 = buf[idx - 3]; /* a(n-2)  */
            seq_t a_nminus4 = buf[idx - 5]; /* a(n-4)  */
            buf[i] = 8 * a_nminus2 - a_nminus4;
        }
    }

    return buf[n - 1];
}
