/*
 * A001165  Position of first even digit after decimal point in sqrt(n).
 * https://oeis.org/A001165
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   search, not a closed form. Generate the decimal digits of
 *            sqrt(n) exactly with the digit-by-digit square-root method and
 *            return the position of the first even digit after the decimal
 *            point
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   a000037_isqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a000037_isqrt.h"

seq_t a(idx_t n) {
    unsigned __int128 r = (unsigned __int128)a000037_isqrt((uint64_t)n);
    unsigned __int128 rem = (unsigned __int128)n - r * r;
    idx_t pos;
    for (pos = 1; pos < 36; pos++) {
        unsigned __int128 t;
        unsigned d;
        rem *= 100;
        d = 9;
        for (;;) {
            t = (20 * r + d) * d;
            if (t <= rem) break;
            d--;
        }
        rem -= t;
        r = 10 * r + d;
        if ((d & 1) == 0) return (seq_t)pos;
    }
    return (seq_t)pos;
}
