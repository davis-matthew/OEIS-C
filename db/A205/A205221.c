/*
 * A205221  Number of (n+1) X 4 0..1 arrays with the number of equal 2 X 2
 *          subblock diagonal pairs and equal antidiagonal pairs differing
 *          from each horizontal or vertical neighbor, and new values 0..1
 *          introduced in row major order.
 * https://oeis.org/A205221
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 20, u64 n <= 44, u128 n <= 93,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Empirical recurrence for A205221:
   a(1)=52, a(2)=132, a(3)=328, a(4)=824, a(5)=2072, a(6)=5176,
   a(7)=13032, a(8)=32488, a(9)=81816
   For n>9:  a(n) = a(n-1)+12*a(n-2)-11*a(n-3)-47*a(n-4)
                    +40*a(n-5)+64*a(n-6)-48*a(n-7)-8*a(n-8) */

#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Guard against the offset being 1; the sequence is undefined for n<1 */
    if (n == 0)
        return 0;

    /* Initial ten terms known from the OEIS entry. */
    static const seq_t init[9] = {
        52,   132,   328,   824,  2072,  5176,
       13032, 32488, 81816
    };

    /* If n is within the initially known range, return it directly. */
    if (n <= 9)
        return init[(size_t)n - 1];

    /* Circular buffer holding the last nine values needed for the recurrence.
       buf[0] => a(n-8), buf[1] => a(n-7), ..., buf[8] => a(n-1). */
    seq_t buf[9];
    for (size_t i = 0; i < 9; ++i)
        buf[i] = init[i];                 /* buf[0] = a(1), ..., buf[8] = a(9) */

    /* Iterate from a(10) up to a(n). */
    for (idx_t cur = 10; cur <= n; ++cur) {
        /* Compute the next value using the recurrence formula. */
        seq_t val = buf[8]                                 /* a(n-1) */
                  + (seq_t)12 * buf[7]                    /* +12*a(n-2) */
                  - (seq_t)11 * buf[6]                    /* -11*a(n-3) */
                  - (seq_t)47 * buf[5]                    /* -47*a(n-4) */
                  + (seq_t)40 * buf[4]                    /* +40*a(n-5) */
                  + (seq_t)64 * buf[3]                    /* +64*a(n-6) */
                  - (seq_t)48 * buf[2]                    /* -48*a(n-7) */
                  - (seq_t)8  * buf[1];                   /* -8*a(n-8) */

        /* Shift the buffer: discard a(n-9) and append the new value. */
        for (size_t i = 0; i < 8; ++i)
            buf[i] = buf[i + 1];
        buf[8] = val;     /* now buf[8] holds a(cur) */
    }

    /* After the loop, buf[8] is a(n). */
    return buf[8];
}
