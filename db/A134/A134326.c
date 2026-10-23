/*
 * A134326  The sum of the elements in the first, middle and last row of the
 *          n-th power of the 9-by-9 matrix defined in the Mathematica
 *          program.
 * https://oeis.org/A134326
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1) + 5*a(n-2) - 23*a(n-3) - 19*a(n-4) + 37*a(n-5) +
 *            42*a(n-6) - 8*a(n-7) - 5*a(n-8) a(0)=3, a(1)=11, a(2)=34,
 *            a(3)=112, a(4)=359, a(5)=1167, a(6)=3764, a(7)=12191
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 17, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *buf = (seq_t *)0;
    static idx_t len = 0;   /* number of values already computed */
    static idx_t cap = 0;   /* current capacity of buf    */

    /* Ensure storage is available.  Allocate an initial capacity of 8. */
    if (!buf)
    {
        cap = 8;
        buf = (seq_t *)malloc((size_t)cap * sizeof(seq_t));
    }

    /* Grow the sequence up to n. */
    while (len <= n)
    {
        /* If we ran out of space, grow the buffer. */
        if (len >= cap)
        {
            idx_t new_cap = cap << 1;
            if (new_cap <= len) new_cap = len + 1;
            seq_t *tmp = (seq_t *)realloc(buf, (size_t)new_cap * sizeof(seq_t));
            buf = tmp;
            cap = new_cap;
        }

        if (len < 8)          /* initialise the first 8 terms explicitly */
        {
            switch (len)
            {
                case 0:  buf[0] = (seq_t)3;  break;
                case 1:  buf[1] = (seq_t)11; break;
                case 2:  buf[2] = (seq_t)34;  break;
                case 3:  buf[3] = (seq_t)112; break;
                case 4:  buf[4] = (seq_t)359; break;
                case 5:  buf[5] = (seq_t)1167; break;
                case 6:  buf[6] = (seq_t)3764; break;
                case 7:  buf[7] = (seq_t)12191; break;
            }
        }
        else
        {
            /* Compute the recurrence in two parts to avoid
               intermediate underflow.  All arithmetic is performed
               in the natural type seq_t. */
            seq_t t = (seq_t)4 * buf[len - 1]
                    + (seq_t)5 * buf[len - 2]
                    + (seq_t)37 * buf[len - 5]
                    + (seq_t)42 * buf[len - 6];

            seq_t s = (seq_t)23 * buf[len - 3]
                    + (seq_t)19 * buf[len - 4]
                    + (seq_t)8  * buf[len - 7]
                    + (seq_t)5  * buf[len - 8];

            buf[len] = t - s;
        }

        ++len;
    }

    return buf[n];
}
