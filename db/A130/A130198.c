/*
 * A130198  Single paradiddle. In percussion, the paradiddle is a four-note
 *          drum sticking pattern consisting of two alternating notes followed
 *          by two notes on the same hand.
 * https://oeis.org/A130198
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-8) = a(n-1) - a(n-4) + a(n-5); initial values:
 *            a(0)=0, a(1)=1, a(2)=0, a(3)=0, a(4)=1, a(5)=0, a(6)=1, a(7)=1
 * Range:     u8 n <= 95, u16 n <= 95, u32 n <= 95, u64 n <= 95, u128 n <= 95,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* 8‑periodic pattern: [0,1,0,0,1,0,1,1] */
    static const seq_t pattern[8] = {0, 1, 0, 0, 1, 0, 1, 1};
    return pattern[n & 7];
}
