/*
 * A219769  Number of n X 4 arrays of the minimum value of corresponding
 *          elements and their horizontal or antidiagonal neighbors in a
 *          random, but sorted with lexicographically nondecreasing rows and
 *          columns, 0..1 n X 4 array.
 * https://oeis.org/A219769
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 12, u32 n <= 50, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A219769
    The number of n×4 arrays of the minimum value of corresponding elements
    and their horizontal or antidiagonal neighbors in a random, but sorted
    with lexicographically nondecreasing rows and columns, 0…1 n×4 array.
    
    Formula used: recurrence valid for n > 14
    a(n) = 9*a(n-1) - 36*a(n-2) + 84*a(n-3) - 126*a(n-4)
          + 126*a(n-5) - 84*a(n-6) + 36*a(n-7) - 9*a(n-8) + a(n-9)

    Initial values (n = 1 … 14):
        4,  9, 33, 100, 283, 732, 1745, 3881, 8149,
        16288, 31169, 57354, 101850, 175100
*/
#include <stdint.h>

seq_t a(idx_t n)
{
    /* static table of first 14 terms (1‑based) */
    static const seq_t init[14] = {
        (seq_t)4,   /* a(1) */
        (seq_t)9,   /* a(2) */
        (seq_t)33,  /* a(3) */
        (seq_t)100, /* a(4) */
        (seq_t)283, /* a(5) */
        (seq_t)732, /* a(6) */
        (seq_t)1745,/* a(7) */
        (seq_t)3881,/* a(8) */
        (seq_t)8149,/* a(9) */
        (seq_t)16288,/* a(10) */
        (seq_t)31169,/* a(11) */
        (seq_t)57354,/* a(12) */
        (seq_t)101850,/* a(13) */
        (seq_t)175100 /* a(14) */
    };

    if (n == 0)
        return (seq_t)0;           /* behaviour below the offset is irrelevant */

    if (n <= 14)
        return init[n - 1];         /* direct lookup for the first 14 terms */

    /*  For n > 14, compute using the 9‑term recurrence
        Keep a circular buffer of the last 9 computed values.
    */
    seq_t buf[9];
    /* initialise buffer with a(6) .. a(14) to start recursion at n = 15 */
    for (idx_t i = 0; i < 9; ++i)
        buf[i] = init[5 + i];

    for (idx_t i = 15; i <= n; ++i)
    {
        seq_t next = 9 * buf[8]
                    - 36 * buf[7]
                    + 84 * buf[6]
                    - 126 * buf[5]
                    + 126 * buf[4]
                    - 84 * buf[3]
                    + 36 * buf[2]
                    - 9 * buf[1]
                    + buf[0];
        /* shift left to discard oldest term and store new term */
        for (idx_t j = 0; j < 8; ++j)
            buf[j] = buf[j + 1];
        buf[8] = next;
    }
    return buf[8];
}
