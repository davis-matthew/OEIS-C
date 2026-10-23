/*
 * A024403  [ (4th elementary symmetric function of S(n))/(3rd elementary
 *          symmetric function of S(n)) ], where S(n) = {first n+3 positive
 *          integers congruent to 2 mod 3}.
 * https://oeis.org/A024403
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor( e4(S(n)) / e3(S(n)) ) where S(n) =
 *            {2,5,8,...,2+3(n+2)} (first n+3 integers ≡2 mod 3) e3 and e4 are
 *            the 3rd and 4th elementary symmetric sums of S(n)
 * Range:     u8 n <= 25, u16 n <= 416, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

typedef unsigned __int128 u128;  /* 128‑bit integer for intermediate results */

seq_t a(idx_t n) {
    /* Size of the set S(n) */
    u128 N = (u128)n + 3;          /* N = n + 3 */

    /* Basic constants of the arithmetic progression */
    const u128 a = 2;              /* first term */
    const u128 d = 3;              /* common difference */

    /* Sums of powers of k = 0 .. N-1 */
    u128 T1 = N * (N - 1) / 2;                                           /* Σk          */
    u128 T2 = N * (N - 1) * (2 * N - 1) / 6;                            /* Σk²        */
    u128 k2 = N * (N - 1) / 2;                                           /* (Σk)²      */
    u128 T3 = k2 * k2;                                                   /* Σk³        */
    u128 T4 = (N - 1) * N * (2 * N - 1) *
             (3 * N * N - 3 * N - 1) / 30;                               /* Σk⁴        */

    /* Power sums of the numbers in S(n) */
    u128 p1 = a * N + d * T1;                                         /* Σxi     */
    u128 p2 = a * a * N + 2 * a * d * T1 + d * d * T2;                /* Σxi²     */
    u128 p3 = a * a * a * N + 3 * a * a * d * T1 +
              3 * a * d * d * T2 + d * d * d * T3;                     /* Σxi³     */
    u128 p4 = a * a * a * a * N +
              4 * a * a * a * d * T1 +
              6 * a * a * d * d * T2 +
              4 * a * d * d * d * T3 +
              d * d * d * d * T4;                                      /* Σxi⁴     */

    /* Elementary symmetric sums from power sums */
    u128 e3 = (p1 * p1 * p1 - 3 * p1 * p2 + 2 * p3) / 6;
    u128 e4 = (p1 * p1 * p1 * p1 - 6 * p1 * p1 * p2 + 3 * p2 * p2 +
               8 * p1 * p3 - 6 * p4) / 24;

    /* Return floor(e4 / e3) as seq_t */
    return (seq_t)(e4 / e3);
}
