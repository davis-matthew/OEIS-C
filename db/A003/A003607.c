/*
 * A003607  Location of 0's when natural numbers are listed in binary.
 * https://oeis.org/A003607
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = position of the (n+1)-th 0 in A030190 (binary expansions
 *            of 0,1,2,... concatenated); found by scanning the bits
 * Range:     u8 n <= 112, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    uint64_t zeros = 0;
    uint64_t pos = 0;
    uint64_t m;

    /* m = 0 is written as the single bit "0" at position 0 */
    if (n == 0) return (seq_t)0;
    zeros = 1;
    pos = 1;

    for (m = 1;; m++) {
        int len = 0;
        uint64_t t = m;
        int b;
        while (t > 0) { len++; t >>= 1; }
        for (b = len - 1; b >= 0; b--) {
            if (((m >> b) & 1) == 0) {
                if (zeros == (uint64_t)n) return (seq_t)pos;
                zeros++;
            }
            pos++;
        }
    }
}
