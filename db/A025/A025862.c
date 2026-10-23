/*
 * A025862  Expansion of 1/((1-x^4)*(1-x^5)*(1-x^10)).
 * https://oeis.org/A025862
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((n^2 + 3*n + 272 + 5*n*(-1)^n + 8*(n+4)*((n+4) mod
 *            5))/400)
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Use int64_t for intermediate arithmetic – the values fit safely for the required n range. */
    int64_t nn = (int64_t)n;          /* cast index to signed for safety */
    int64_t sum = 0;

    /* n^2 + 3n + 272 */
    sum += nn * nn;
    sum += 3 * nn;
    sum += 272;

    /* 5*n*(-1)^n */
    if (n & 1)          /* n is odd → (-1)^n = -1 */
        sum -= 5 * nn;   /* subtract 5n */
    else
        sum += 5 * nn;   /* add 5n */

    /* 8*(n+4)*((n+4) mod 5) */
    idx_t base = n + 4;
    idx_t rem  = base % 5;
    sum += 8LL * (int64_t)base * (int64_t)rem;

    /* floor division by 400 */
    seq_t result = (seq_t)(sum / 400);
    return result;
}
