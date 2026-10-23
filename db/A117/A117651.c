/*
 * A117651  A002415 and A052472 interlaced.
 * https://oeis.org/A117651
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, less, easy
 * Formula:   a(n) = abs((2*n^4 +12*n^3 -2*n^2 -132*n -195 + (4*n^3 -6*n^2
 *            -124*n -189)*(-1)^n)) / 384 for n = 0,1,2,
 * Range:     u8 n <= 13, u16 n <= 57, u32 n <= 951, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Work with signed 64‑bit intermediates; the values fit
       for all n that appear in the tests (≤ 10 000).  */
    int64_t nn = (int64_t)n;
    int64_t sign = (n & 1) ? -1 : 1; /* (-1)^n */

    /* 2*n^4 + 12*n^3 - 2*n^2 - 132*n - 195 */
    int64_t term1 = 2*nn*nn*nn*nn
                  + 12*nn*nn*nn
                  - 2*nn*nn
                  - 132*nn
                  - 195;

    /* (4*n^3 - 6*n^2 - 124*n - 189) * (-1)^n */
    int64_t term2 = (4*nn*nn*nn
                  - 6*nn*nn
                  - 124*nn
                  - 189) * sign;

    int64_t num = term1 + term2;
    if (num < 0) num = -num;       /* absolute value */
    return (seq_t)(num / 384);     /* exact division */
}
