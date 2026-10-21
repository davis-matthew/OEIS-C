/*
 * A087328  Independence numbers for KT_3 knight on hexagonal board.
 * https://oeis.org/A087328
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ceil(n^2/4) + adjustment adjustment = 0 if n mod 12 ∈
 *            {0,1,4,8,11} +1 if n mod 12 ∈ {3,9} +2 if n mod 12 ∈
 *            {2,5,6,7,10} except when n = 6 +3 if n = 6
 * Range:     u8 n <= 31, u16 n <= 511, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    seq_t nn = (seq_t)n;
    /* ceil(n^2 / 4) = (n^2 + 3) / 4 for positive n */
    seq_t base = ((nn * nn) + (seq_t)3) / (seq_t)4;

    if (n == 6u) {
        /* special case for n = 6 */
        return base + (seq_t)3;
    }

    switch (n % 12u) {
        case 0u:
        case 1u:
        case 4u:
        case 8u:
        case 11u:
            return base; /* no adjustment */
        case 3u:
        case 9u:
            return base + (seq_t)1;
        default: /* 2,5,7,10 (and 6 when n > 6) */
            return base + (seq_t)2;
    }
}
