/*
 * A024390  [ (4th elementary symmetric function of S(n))/(3rd elementary
 *          symmetric function of S(n)) ], where S(n) = {first n+3 positive
 *          integers congruent to 1 mod 4}.
 * https://oeis.org/A024390
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor( e4(S(n)) / e3(S(n)) ), where e_k is the k‑th
 *            elementary symmetric sum of the first n+3 positive integers
 *            congruent to 1 mod 4 (i.e. 1,5,9,13,…)
 * Range:     u8 n <= 21, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    idx_t m = n + 3;                /* number of elements in S(n) */
    unsigned __int128 e1 = 0, e2 = 0, e3 = 0, e4 = 0;

    for (idx_t i = 0; i < m; ++i) {
        unsigned __int128 x = (unsigned __int128)(1 + 4 * i);
        /* update elementary symmetric sums from highest to lowest */
        e4 += x * e3;
        e3 += x * e2;
        e2 += x * e1;
        e1 += x;
    }

    return (seq_t)(e4 / e3);   /* floor of the rational e4/e3 */
}
