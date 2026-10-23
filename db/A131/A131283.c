/*
 * A131283  a(n) is the number of binary strings of length n such that there
 *          exist 3 or more ones in a subsequence of length 5 or less.
 * https://oeis.org/A131283
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A131283
    a(n) = 3*a(n-1) - 2*a(n-2) + a(n-3) - 2*a(n-4) + 2*a(n-5)
           - 4*a(n-6)      - a(n-8)   + 2*a(n-9)  - a(n-10) + 2*a(n-11)
    initial values: a(1)=0, a(2)=0, a(3)=1, a(4)=5, a(5)=16, a(6)=38,
                    a(7)=85, a(8)=185, a(9)=396, a(10)=838, a(11)=1748   */
#include <stddef.h>

static const seq_t init[12] = {0,   /* index 0 unused          */
                               0,   /* a(1)                     */
                               0,   /* a(2)                     */
                               1,   /* a(3)                     */
                               5,   /* a(4)                     */
                               16,  /* a(5)                     */
                               38,  /* a(6)                     */
                               85,  /* a(7)                     */
                               185, /* a(8)                     */
                               396, /* a(9)                     */
                               838, /* a(10)                    */
                               1748 /* a(11)                    */};

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                   /* a(0) is undefined; harmless */
    if (n < 12)                      /* use initial values directly */
        return init[n];

    seq_t buf[12];
    /* initialise the circular buffer with the first 11 terms   */
    for (size_t i = 1; i < 12; ++i)
        buf[i] = init[i];

    for (idx_t cur = 12; cur <= n; ++cur)
    {
        /* compute a(cur) from the recurrence relation         */
        seq_t res =
            3 * buf[11]               /* a(cur-1)  */
          - 2 * buf[10]               /* a(cur-2)  */
          +   buf[9]                 /* a(cur-3)  */
          - 2 * buf[8]               /* a(cur-4)  */
          + 2 * buf[7]               /* a(cur-5)  */
          - 4 * buf[6]               /* a(cur-6)  */
          -   buf[4]                 /* a(cur-8)  */
          + 2 * buf[3]               /* a(cur-9)  */
          -   buf[2]                 /* a(cur-10) */
          + 2 * buf[1];              /* a(cur-11) */

        /* shift buffer left by one position (discard oldest) */
        for (size_t j = 1; j < 11; ++j)
            buf[j] = buf[j + 1];

        buf[11] = res;               /* newest term                */
    }
    return buf[11];                  /* a(n) is now in buf[11]    */
}
