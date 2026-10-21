/*
 * A085583  Number of (3412,1234)-avoiding involutions in S_n.
 * https://oeis.org/A085583
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*n^4-4*n^3+28*n^2-2*n+81-6*n*(-1)^n+15*(-1)^n)/96
 * Range:     u8 n <= 10, u16 n <= 42, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* Use signed __int128 for safe intermediate arithmetic. */
    __int128 N  = (__int128)n;
    __int128 n2 = N * N;
    __int128 n3 = n2 * N;
    __int128 n4 = n3 * N;

    /* Base part of the numerator: 2*n^4 - 4*n^3 + 28*n^2 - 2*n + 81 */
    __int128 base = 2 * n4 - 4 * n3 + 28 * n2 - 2 * N + 81;

    /* Alternating part: (15 - 6*n) * (-1)^n */
    __int128 alt = 15 - 6 * N;
    if ((N & 1) == 0) {               /* n even: (-1)^n = 1 */
        /* alt stays as (15 - 6*n) */
    } else {                           /* n odd: (-1)^n = -1 */
        alt = -alt;                    /* alt = 6*n - 15 */
    }

    __int128 numerator = base + alt;   /* Final numerator */
    return (seq_t)(numerator / 96);    /* Division is exact for all n ≥ 1 */
}
