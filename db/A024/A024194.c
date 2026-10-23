/*
 * A024194  [ (4th elementary symmetric function of S(n))/(2nd elementary
 *          symmetric of S(n)) ], where S(n) = {3,4, ..., n+5}.
 * https://oeis.org/A024194
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor( (4th elementary symmetric function of S(n))
 *            divided by the 2nd elementary symmetric function of S(n) ),
 *            where S(n) = {3,4,...,n+5}
 * Range:     u8 n <= 7, u16 n <= 38, u32 n <= 670, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

typedef unsigned __int128 u128;

/* Computes a(n) for n >= 1. */
seq_t a(idx_t n)
{
    if (n < 1u) return 0u;   /* undefined for n=0, but safe fallback */

    /* Size of the set S(n) = n + 3. */
    idx_t r = n + 3u;        /* number of terms in the set */

    /* Work in 128‑bit to avoid overflow for intermediate values. */
    u128 R = (u128)r;

    /* Power sums of the arithmetic progression 3,4,...,n+5. */
    u128 p1 = R * (R + 5u) / 2u;                           /* sum of the elements */

    u128 p2 = 9u * R
             + 3u * R * (R - 1u)
             + (R - 1u) * R * (2u * R - 1u) / 6u;         /* sum of squares */

    u128 tmp3 = (R - 1u) * R / 2u;
    u128 p3 = 27u * R
              + 27u * (R - 1u) * R / 2u
              + 3u * (R - 1u) * R * (2u * R - 1u) / 2u
              + tmp3 * tmp3;                               /* sum of cubes */

    u128 term4a = 81u * R;
    u128 term4b = 54u * (R - 1u) * R;
    u128 term4c = 9u * (R - 1u) * R * (2u * R - 1u);
    u128 term4d = 3u * (R - 1u) * (R - 1u) * R * R;
    u128 term4e_num = (R - 1u) * R * (2u * R - 1u)
                      * (3u * R * R - 3u * R - 1u);
    u128 term4e = term4e_num / 30u;
    u128 p4 = term4a + term4b + term4c + term4d + term4e; /* sum of fourth powers */

    /* Elementary symmetric sums via Newton's identities. */
    u128 e2 = (p1 * p1 - p2) / 2u;                          /* 2nd symmetric sum */
    u128 e3 = (p1 * e2 - p2 * p1 + p3) / 3u;               /* 3rd symmetric sum */
    u128 e4 = (p1 * e3 - p2 * e2 + p3 * p1 - p4) / 4u;     /* 4th symmetric sum */

    /* a(n) is the integer division of e4 by e2 (truncating). */
    u128 result = e4 / e2;
    return (seq_t)result;
}
