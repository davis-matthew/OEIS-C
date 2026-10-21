/*
 * A060248  Numbers that begin with the letter "d" in Portuguese.
 * https://oeis.org/A060248
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, word
 * Formula:   not stated
 * Range:     u8 n <= 53, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   is_d
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/is_d.h"
/* Sequence: numbers whose Portuguese word starts with the letter 'd'.
   The function a(n) returns the nth such number (offset 1). */

seq_t a(idx_t n)
{
    seq_t val   = 1;   /* candidate integer */
    idx_t count = 0;   /* how many d‑numbers found so far */

    while (1) {
        if (is_d(val)) {
            ++count;
            if (count == n)
                return val;
        }
        ++val;
    }
}
