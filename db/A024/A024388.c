/*
 * A024388  [ (3rd elementary symmetric function of S(n))/(2nd elementary
 *          symmetric function of S(n)) ], where S(n) = {first n+2 positive
 *          integers congruent to 1 mod 4}.
 * https://oeis.org/A024388
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A024388
 *  a(n) = � E3(S(n+1)) / E2(S(n+1)) �
 *  where S(k) = {1,5,9,…,4(k-1)+1} – first k+2 numbers ≡1 (mod4)
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* use unsigned __int128 for intermediate calculations to avoid overflow */
    unsigned __int128 N = (unsigned __int128)n + 3;            /* size of the set: n+3 elements */
    unsigned __int128 p1 = 2 * N * N - N;                    /* Σx_i */

    /* Σx_i^2  */
    unsigned __int128 t1 = (unsigned __int128)8 * (N - 1) * N * (2 * N - 1) / 3;
    unsigned __int128 t2 = (unsigned __int128)4 * N * (N - 1);
    unsigned __int128 p2 = t1 + t2 + N;                      /* used later in Newton's identities */

    /* e2 = (p1^2 - p2)/2  */
    unsigned __int128 e2 = (p1 * p1 - p2) / 2;

    /* Σx_i^3  */
    unsigned __int128 t3_1 = (unsigned __int128)16 * N * N * (N - 1) * (N - 1);
    unsigned __int128 t3_2 = (unsigned __int128)8 * (N - 1) * N * (2 * N - 1);
    unsigned __int128 t3_3 = (unsigned __int128)6 * N * (N - 1);
    unsigned __int128 p3 = t3_1 + t3_2 + t3_3 + N;

    /* e3 = (p3 - p1*p2 + e2*p1)/3  (by Newton's identities) */
    unsigned __int128 e3 = (p3 - p1 * p2 + e2 * p1) / 3;

    /* return floor(e3 / e2) */
    return (seq_t)(e3 / e2);
}
