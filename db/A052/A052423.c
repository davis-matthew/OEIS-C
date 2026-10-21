/*
 * A052423  Highest common factor of nonzero digits of n.
 * https://oeis.org/A052423
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = GCD of all non‑zero decimal digits of n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n) {
    /* Extract the decimal representation of n (idx_t is uint64_t). */
    uint64_t x = (uint64_t)n;
    seq_t g = 0;                    /* GCD of digits found so far */
    while (x > 0) {
        uint64_t d = x % 10;        /* next decimal digit */
        x /= 10;
        if (d == 0) continue;       /* ignore zero digits */
        seq_t digit = (seq_t)d;
        if (g == 0) {               /* first non‑zero digit */
            g = digit;
        } else {                    /* update GCD with this digit */
            seq_t a_local = g;
            seq_t b_local = digit;
            while (b_local != 0) {
                seq_t temp = b_local;
                b_local = a_local % b_local;
                a_local = temp;
            }
            g = a_local;
        }
    }
    return g;
}
