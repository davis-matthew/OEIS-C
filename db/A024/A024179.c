/*
 * A024179  Integer part of ((4th elementary symmetric function of
 *          2,3,...,n+4)/(2+3+...+n+4)).
 * https://oeis.org/A024179
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor((n*(n+1)*(n+2)*(15*n^4 + 330*n^3 + 2765*n^2 +
 *            10482*n + 15208))/(2880*(n+6)))
 * Range:     u8 n <= 3, u16 n <= 12, u32 n <= 93, u64 n <= 3901,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Indexed starting at n = 1 (OEIS offset 1). */

#include <stdint.h>

seq_t a(idx_t n) {
    /* cast index to seq_t */
    seq_t N = (seq_t)n;
    /* compute 128-bit intermediate values */
    unsigned __int128 n128 = (unsigned __int128)N;
    unsigned __int128 n1 = n128 + 1;
    unsigned __int128 n2 = n128 + 2;
    unsigned __int128 n6 = n128 + 6;

    /* big polynomial P = 15*n^4 + 330*n^3 + 2765*n^2 + 10482*n + 15208 */
    unsigned __int128 p128
        = 15 * (n128 * n128 * n128 * n128)
        + 330 * (n128 * n128 * n128)
        + 2765 * (n128 * n128)
        + 10482 * n128
        + 15208;

    /* numerator = n*(n+1)*(n+2)*P */
    unsigned __int128 num = n128 * n1 * n2 * p128;

    /* denominator = 2880*(n+6) */
    unsigned __int128 denom = 2880 * n6;

    /* floor division */
    unsigned __int128 result128 = num / denom;

    /* cast back to seq_t (truncates if out of range; exact when it fits) */
    return (seq_t)result128;
}
