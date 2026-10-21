/*
 * A227085  Number of n X 2 binary arrays indicating whether each 2 X 2
 *          subblock of a larger binary array has lexicographically increasing
 *          rows and columns, for some larger (n+1) X 3 binary array with rows
 *          and columns of the latter in lexicographically nondecreasing
 *          order.
 * https://oeis.org/A227085
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n^5 + 5*n^4 + 25*n^3 + 175*n^2 + 154*n + 120) / 120
 *            (valid for n >= 1, where a(1)=4, a(2)=12, etc.)
 * Range:     u8 n <= 6, u16 n <= 22, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* ensure n >= 1; for n == 0 the sequence is not defined by OEIS */
    if (n == 0) {
        return (seq_t)0;   /* defensive; not used by tests */
    }

    /* compute using 128-bit intermediate to avoid overflow on 64-bit builds */
    __int128 nn   = (__int128)n;
    __int128 num = 0;

    num += nn * nn * nn * nn * nn;          /* n^5          */
    num += 5  * nn * nn * nn * nn;          /* 5 n^4        */
    num += 25 * nn * nn * nn;              /* 25 n^3       */
    num += 175 * nn * nn;                  /* 175 n^2      */
    num += 154 * nn;                       /* 154 n        */
    num += 120;                            /* +120         */

    num /= 120;                            /* final division */

    return (seq_t)num;
}
