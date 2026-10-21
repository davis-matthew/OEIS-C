/*
 * A239096  a(n) = (n^9 + 21*n^5 - 190*n^3 + 168*n)/1260.
 * https://oeis.org/A239096
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^9 + 21*n^5 - 190*n^3 + 168*n)/1260
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 25, u64 n <= 305, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 3u)
        return 0;

    seq_t product = 1;
    int c2 = 2, c3 = 2, c5 = 1, c7 = 1;

    /* cancel factors of the denominator from each numerator factor */
    #define CANCELLATION(_v)                          \
    {                                                  \
        seq_t _f = (_v);                               \
        while (c2 && (_f % 2 == 0)) { _f /= 2; c2--; } \
        while (c3 && (_f % 3 == 0)) { _f /= 3; c3--; } \
        while (c5 && (_f % 5 == 0)) { _f /= 5; c5--; } \
        while (c7 && (_f % 7 == 0)) { _f /= 7; c7--; } \
        product *= _f;                                 \
    }

    seq_t nn = (seq_t)n;

    CANCELLATION(nn - 2);  /* (n‑2) */
    CANCELLATION(nn - 1);  /* (n‑1) */
    CANCELLATION(nn);      /*  n   */
    CANCELLATION(nn + 1);  /* (n+1) */
    CANCELLATION(nn + 2);  /* (n+2) */

    /* polynomial factor n^4 + 5*n^2 + 42 */
    seq_t poly = nn * nn * nn * nn + (seq_t)5 * nn * nn + 42;
    CANCELLATION(poly);

    #undef CANCELLATION

    /* all denominator factors have been cancelled */
    return product;
}
