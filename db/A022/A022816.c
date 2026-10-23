/*
 * A022816  Number of terms in 6th derivative of a function composed with
 *          itself n times.
 * https://oeis.org/A022816
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+1)*(n^3+24*n^2+81*n-46)/120
 * Range:     u8 n <= 2, u16 n <= 19, u32 n <= 215, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"   /* provides seq_t gcd(seq_t a, seq_t b) */

seq_t a(idx_t n)
{
    if (n == 0)                /* below the OEIS offset – not used */
        return 0;

    seq_t N   = (seq_t)n;      /* cast once – all following work in seq_t */
    seq_t a1  = N;
    seq_t b1  = N + 1;

    /* cubic polynomial n^3 + 24 n^2 + 81 n - 46  */
    seq_t N2  = N * N;                 /* n^2      */
    seq_t N3  = N2 * N;                /* n^3      */
    seq_t part1 = N3;
    seq_t part2 = N2 * (seq_t)24;      /* 24 n^2   */
    seq_t part3 = N  * (seq_t)81;      /* 81 n     */
    seq_t c = part1 + part2 + part3 - (seq_t)46;

    seq_t denom = (seq_t)120;          /* 120 = 8·3·5 */

    /* Cancel factors of 120 between the three multiplicative parts
       to avoid any intermediate overflow on a 64‑bit implementation */
    seq_t g = gcd(a1, denom);
    denom /= g;
    a1    /= g;

    g = gcd(b1, denom);
    denom /= g;
    b1    /= g;

    g = gcd(c, denom);
    denom /= g;
    c    /= g;

    /* After cancellations denom should be 1 (the result is integer) */
    return (a1 * b1) * c;
}
