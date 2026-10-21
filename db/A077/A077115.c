/*
 * A077115  Least integer square >= n^3.
 * https://oeis.org/A077115
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ceil(n^(3/2))^2 The smallest perfect square not less than
 *            n³ is obtained by computing the integer cube n³, taking its
 *            floor square root s, and squaring s if s² == n³, otherwise
 *            squaring s+1
 * Range:     u8 n <= 6, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   floor_sqrt_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/floor_sqrt_seq.h"

seq_t a(idx_t n) {
    /* compute n³, using seq_t to avoid overflow in arbitrary‑precision builds */
    seq_t cube = (seq_t)n * (seq_t)n * (seq_t)n;

    /* floor of sqrt(cube) */
    seq_t s = floor_sqrt_seq(cube);

    /* if s² equals cube we already have the exact square, else use next square */
    seq_t sq = s * s;
    if (sq == cube)
        return cube;
    else {
        seq_t next = s + 1;
        return next * next;
    }
}
