/*
 * A235443  Number of (n+1) X (2+1) 0..1 arrays with the difference between
 *          each 2 X 2 subblock maximum and minimum lexicographically
 *          nondecreasing rowwise and columnwise.
 * https://oeis.org/A235443
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 8*a(n-1) - 8*a(n-2) - 16*a(n-3) + 12*a(n-4) + 14*a(n-5) -
 *            a(n-6) - 2*a(n-7) a(1)=58 , a(2)=382 , a(3)=2476 , a(4)=15936 ,
 *            a(5)=102376 , a(6)=657290 , a(7)=4219322
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 10, u64 n <= 22, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t */

seq_t a(idx_t n)
{
    /* All values are defined only for n ≥ 1.  For convenience return 0 for
       n == 0, but such a call is outside the OEIS offset and is not
       required by the tests. */
    if (n == 0) return 0;

    /* Initial seven terms – hard‑coded because they are required by the
       recurrence.  Keeping them local avoids the need for a heap
       allocation of arbitrary‑precision objects. */
    static const seq_t init[8] = {
        0ull,                 /* unused index 0 */
        58ull,                /* a(1) */
        382ull,               /* a(2) */
        2476ull,              /* a(3) */
        15936ull,             /* a(4) */
        102376ull,            /* a(5) */
        657290ull,            /* a(6) */
        4219322ull            /* a(7) */
    };

    /* For n within the first seven terms, simply return the constant. */
    if (n <= 7) return init[n];

    /* Recurrence buffer.  We keep the last 7 computed terms in a circular
       fashion to avoid overflow of compilers for large loops. */
    seq_t buf[7];
    buf[0] = init[1];
    buf[1] = init[2];
    buf[2] = init[3];
    buf[3] = init[4];
    buf[4] = init[5];
    buf[5] = init[6];
    buf[6] = init[7];

    /* Compute from a(8) up to a(n). */
    for (idx_t k = 8; k <= n; ++k) {
        /* Indexing into the buffer: we always write into buf[0]
           and then shift the buffer.  The recurrence wants the
           previous seven values in order a(k-1) … a(k-7). */
        seq_t next = 8   * buf[6]        /* a(k-1) */
                   - 8   * buf[5]        /* a(k-2) */
                   - 16  * buf[4]        /* a(k-3) */
                   + 12  * buf[3]        /* a(k-4) */
                   + 14  * buf[2]        /* a(k-5) */
                   -     buf[1]         /* a(k-6) */
                   - 2   * buf[0];       /* a(k-7) */

        /* Shift buffer: a(k-7) becomes a(k-6), etc. */
        buf[0] = buf[1];
        buf[1] = buf[2];
        buf[2] = buf[3];
        buf[3] = buf[4];
        buf[4] = buf[5];
        buf[5] = buf[6];
        buf[6] = next;
    }

    /* After the loop buf[6] holds a(n). */
    return buf[6];
}
