/*
 * A218037  Decimal numbers with exactly two consecutive zeros.
 * https://oeis.org/A218037
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Decimal numbers whose decimal representation has exactly
   two consecutive zeros and no other zero digits. */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;           /* Undefined for n < 1 */

    idx_t target = n;
    idx_t found  = 0;
    idx_t x      = 1;

    for (;;) {
        unsigned last = (unsigned)(x % 10); /* least significant digit */
        int zeroCount = (last == 0) ? 1 : 0;
        int pairCount = 0;
        idx_t tmp = x / 10;

        while (tmp > 0) {
            unsigned d = (unsigned)(tmp % 10);

            if (d == 0) {
                zeroCount++;
                if (last == 0) {
                    pairCount++;
                    if (pairCount > 1) break;  /* more than one pair */
                }
                if (zeroCount > 2) break;  /* more than two zeros */
            }

            last = d;
            tmp /= 10;
        }

        if (pairCount == 1 && zeroCount == 2) {
            ++found;
            if (found == target) return (seq_t)x;
        }
        ++x;
    }
}
