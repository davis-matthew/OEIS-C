/*
 * A026918  Sum{T(n-k,k)}, 0<=k<=[ n/2 ], T given by A026907.
 * https://oeis.org/A026918
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*Fibonacci(n+5) + (3/4)*[(n-1)(-1)^n - 17(n+3)]
 * Range:     u8 n <= 4, u16 n <= 15, u32 n <= 29, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/fib.h"

seq_t a(idx_t n)
{
    /* fibonacci part */
    seq_t fn5 = fib(n + 5);           /* fib(n+5) */
    seq_t part1 = 8 * fn5;            /* 8 * fib(n+5) */

    /* compute B = 17*(n+3) - (n-1)*(-1)^n   (always positive) */
    seq_t B;
    if ((n & 1) == 0) { /* n even, (-1)^n = +1 */
        if (n == 0) {
            B = 52;                 /* 17*3 - (-1) = 52 */
        } else {
            B = 17 * ((seq_t)n + 3) - ((seq_t)n - 1);
        }
    } else {                  /* n odd, (-1)^n = -1 */
        B = 17 * ((seq_t)n + 3) + ((seq_t)n - 1);
    }

    /* B is a multiple of 4, so (B/4)*3 is integer */
    seq_t B_div4 = B >> 2;          /* divide by 4 */
    seq_t part2 = 3 * B_div4;       /* 3*(B/4) */

    return part1 - part2;
}
