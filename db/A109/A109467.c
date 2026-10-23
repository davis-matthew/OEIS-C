/*
 * A109467  4 X 4 analog of A094943.
 * https://oeis.org/A109467
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n+4) = 4*a(n+3) + 44*a(n+2) + 144*a(n+1) + 160*a(n), initial
 *            values: a(0)=1, a(1)=26, a(2)=256, a(3)=2472
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *buf = NULL;
    static idx_t cap = 0;      /* allocated size of buf */
    static idx_t last = 0;     /* index of the last computed term */

    /* First call: allocate and store the initial four values */
    if (buf == NULL)
    {
        cap = 4;
        buf = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!buf)
            return (seq_t)0;    /* out of memory; unlikely in tests */
        buf[0] = (seq_t)1;
        buf[1] = (seq_t)26;
        buf[2] = (seq_t)256;
        buf[3] = (seq_t)2472;
        last = 3;              /* we now have a(0) ... a(3) */
    }

    /* If the requested index is already computed, return it */
    if (n <= last)
        return buf[n];

    /* Grow the buffer until it can hold up to n */
    while (last < n)
    {
        /* Ensure enough space for the next element */
        if ((size_t)(last + 1) >= cap)
        {
            size_t newcap = cap * 2;
            seq_t *tmp = (seq_t *)realloc(buf, newcap * sizeof(seq_t));
            if (!tmp)
                return (seq_t)0;    /* allocation failure */
            buf = tmp;
            cap = newcap;
        }

        /* Compute a(last+1) using the recurrence */
        seq_t val =
            (seq_t)4   * buf[last]   /* a(n-1) */
          + (seq_t)44  * buf[last-1] /* a(n-2) */
          + (seq_t)144 * buf[last-2] /* a(n-3) */
          + (seq_t)160 * buf[last-3];/* a(n-4) */

        buf[++last] = val;
    }

    return buf[n];
}
