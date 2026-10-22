/*
 * A002714  Number of different keys with n cuts, depths between 1 and 7 and
 *          depth difference at most 1 between adjacent cut depths.
 * https://oeis.org/A002714
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of length-n words over {1..7} with adjacent
 *            letters differing by at most 1; computed by DP c_j(n) =
 *            c_{j-1}(n-1) + c_j(n-1) + c_{j+1}(n-1), c_j(1) = 1, a(n) = Sum_j
 *            c_j(n), a(0) = 1. Equivalently a(n) = 4*a(n-1) - 2*a(n-2) -
 *            4*a(n-3) + a(n-4) for n >= 5
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 20, u64 n <= 41, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n) {
    seq_t c[7], d[7];
    seq_t s;
    idx_t i;
    int j;

    if (n == 0) return (seq_t)1;

    for (j = 0; j < 7; j++) c[j] = (seq_t)1;

    for (i = 1; i < n; i++) {
        for (j = 0; j < 7; j++) {
            seq_t t = c[j];
            if (j > 0) t += c[j - 1];
            if (j < 6) t += c[j + 1];
            d[j] = t;
        }
        for (j = 0; j < 7; j++) c[j] = d[j];
    }

    s = (seq_t)0;
    for (j = 0; j < 7; j++) s += c[j];
    return s;
}
