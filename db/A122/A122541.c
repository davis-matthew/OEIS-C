/*
 * A122541  Squares that can be seen on a digital clock.
 * https://oeis.org/A122541
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, base
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 33, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sieve all squares 0 <= s^2 <= 2359 and keep those representing
   valid 24‑hour times HHMM. a(n) returns the nth such square
   (offset 1). */
#include <stddef.h>

seq_t a(idx_t n)
{
    static seq_t seq[50];
    static size_t sz = 0;

    /* initialize once */
    if (sz == 0) {
        size_t i = 0;
        for (idx_t s = 0; s <= 48; ++s) {
            idx_t sq = s * s;
            if (sq > 2359) break;
            idx_t minutes = sq % 100;
            idx_t hours   = sq / 100;
            if (minutes < 60 && hours < 24) {
                seq[i++] = (seq_t)sq;
            }
        }
        sz = i;
    }

    if (n == 0 || n > sz) return 0;
    return seq[n - 1];
}
