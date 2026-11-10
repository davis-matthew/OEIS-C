/*
 * A051066  Partial sums of A051065.
 * https://oeis.org/A051066
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   a(n) = Sum_{k=0..n} A051065(k), where A051065(k) = (k +
 *            A051065(floor(k/3))) mod 2, A051065(0) = 0
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Validated: partial, pass, 75 terms through n = 74 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static unsigned A051065_a(idx_t n) {
    unsigned r = 0;
    /* Unroll the recursion: a(n) = n + a(n/3) mod 2 */
    while (n > 0) {
        r ^= (unsigned)(n & 1);
        n /= 3;
    }
    return r;
}

seq_t a(idx_t n) {
    seq_t s = 0;
    idx_t k;
    for (k = 1; k <= n; k++) {
        if (A051065_a(k))
            s++;
    }
    return s;
}
