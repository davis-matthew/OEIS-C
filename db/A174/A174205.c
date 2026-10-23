/*
 * A174205  Write natural numbers in base 2 as a stream of digits. Moving left
 *          to right, delete odd occurrences of digit 0 and 1.
 * https://oeis.org/A174205
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 133, u16 n <= 133, u32 n <= 133, u64 n <= 133,
 *            u128 n <= 133, big any n
 * Helpers:   floor_log2_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Definition: a(n) is the nth bit of the infinite stream obtained by
   concatenating the binary representations of the natural numbers
   (0,1,10,11,100,...) and then deleting the 1st,3rd,5th,... occurrences
   of each digit.  The function returns the value at position n
   (offset 1). */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/floor_log2_uint64.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* safety; offset 1 in OEIS */

    idx_t cnt0 = 0, cnt1 = 0;       /* counts of 0's and 1's seen so far */
    idx_t out = 0;                  /* number of surviving bits emitted */
    idx_t m = 0;                    /* current natural number */

    for (;;) {
        /* Emit the binary digits of m from most significant to least. */
        if (m == 0) {
            /* Special case: "0" has a single digit '0'. */
            ++cnt0;
            if ((cnt0 & 1) == 0) {  /* keep if this is an even occurrence */
                ++out;
                if (out == n) return (seq_t)0;
            }
        } else {
            idx_t msb = floor_log2_uint64(m);
            for (idx_t i = msb; ; --i) {
                idx_t bit = (m >> i) & 1;
                if (bit == 0) {
                    ++cnt0;
                    if ((cnt0 & 1) == 0) {
                        ++out;
                        if (out == n) return (seq_t)0;
                    }
                } else {          /* bit == 1 */
                    ++cnt1;
                    if ((cnt1 & 1) == 0) {
                        ++out;
                        if (out == n) return (seq_t)1;
                    }
                }
                if (i == 0) break;
            }
        }
        ++m;
    }
}
