/*
 * A002847  Number of ways of getting a straight flush, 4 of a kind, full
 *          house, flush, straight, 3 of a kind, 2 pair, a pair, no pair in
 *          poker.
 * https://oeis.org/A002847
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, nonn, full, nice
 * Formula:   counts of 5-card poker hands by class, from binomials: a(1) =
 *            10*4 (straight flushes), a(2) = 13*48, a(3) =
 *            13*12*C(4,3)*C(4,2), a(4) = 4*C(13,5) - 40, a(5) = 10*4^5 - 40,
 *            a(6) = 13*C(12,2)*C(4,3)*4^2, a(7) = C(13,2)*C(4,2)^2*44, a(8) =
 *            13*C(12,3)*C(4,2)*4^3, a(9) = (C(13,5) - 10)*(4^5 - 4)
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Helpers:   a002847_binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/a002847_binom.h"

seq_t a(idx_t n) {
    seq_t straights = 10;                 /* rank sequences A-5 .. 10-A */
    seq_t sf = straights * 4;             /* straight flushes */
    seq_t suits5 = (seq_t)4 * 4 * 4 * 4 * 4;
    seq_t r;
    switch (n) {
    case 1:
        r = sf;
        break;
    case 2:
        r = (seq_t)13 * a002847_binom(4, 4) * (seq_t)48;
        break;
    case 3:
        r = (seq_t)13 * a002847_binom(4, 3) * (seq_t)12 * a002847_binom(4, 2);
        break;
    case 4:
        r = (seq_t)4 * a002847_binom(13, 5) - sf;
        break;
    case 5:
        r = straights * suits5 - sf;
        break;
    case 6:
        r = (seq_t)13 * a002847_binom(4, 3) * a002847_binom(12, 2) * (seq_t)16;
        break;
    case 7:
        r = a002847_binom(13, 2) * a002847_binom(4, 2) * a002847_binom(4, 2) * (seq_t)44;
        break;
    case 8:
        r = (seq_t)13 * a002847_binom(4, 2) * a002847_binom(12, 3) * (seq_t)64;
        break;
    case 9:
        r = (a002847_binom(13, 5) - straights) * (suits5 - 4);
        break;
    default:
        r = 0;
        break;
    }
    return r;
}
