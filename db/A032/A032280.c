/*
 * A032280  Number of bracelets (turnover necklaces) of n beads of 2 colors, 7
 *          of them black.
 * https://oeis.org/A032280
 *
 * Offset:    7    (a(7) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (48*C(n-1,6) + 7*(n-2)*(n-4)*(n-6) + 288*([n%7==0])) /
 *            672 (use (n-1)*(n-3)*(n-5) instead of (n-2)*(n-4)*(n-6) when n
 *            is odd)
 * Range:     u8 n <= 15, u16 n <= 33, u32 n <= 190, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 7) return 0;          /* values below offset are undefined */

    /* use 128‑bit integer for intermediate calculations */
    unsigned __int128 N = n;

    /* product of six consecutive numbers (n-1)(n-2)...(n-6)  */
    unsigned __int128 prod6 = (N-1)*(N-2)*(N-3)*(N-4)*(N-5)*(N-6);

    /* first part: 48*C(n-1,6) = prod6 / 15  (since 48/720 = 1/15) */
    unsigned __int128 part1 = prod6 / 15;

    /* third part depends on parity of n */
    unsigned __int128 prod3;
    if ((n & 1) == 0)               /* n even : (n-2)(n-4)(n-6) */
        prod3 = (N-2)*(N-4)*(N-6);
    else                             /* n odd  : (n-1)(n-3)(n-5) */
        prod3 = (N-1)*(N-3)*(N-5);

    /* second part: 7 * prod3 */
    unsigned __int128 part2 = 7 * prod3;

    /* fourth part: 288 when n is a multiple of 7, else 0 */
    unsigned __int128 part3 = (n % 7 == 0) ? 288 : 0;

    /* total numerator (exactly divisible by 672) */
    unsigned __int128 num = part1 + part2 + part3;

    /* final result */
    return (seq_t)(num / 672);
}
