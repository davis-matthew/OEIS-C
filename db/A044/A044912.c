/*
 * A044912  Numbers whose base-11 run lengths alternate: odd, even, odd, ...
 * https://oeis.org/A044912
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 23, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   _a044912_is_valid
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/_a044912_is_valid.h"

/* OEIS A044912: numbers whose base‑11 run lengths alternate odd, even, odd, ...
   Algorithm: check each integer in order; a number is valid if the runs of
   equal digits in its base‑11 expansion start with an odd length and then
   alternate parity.                                                        */

seq_t a(idx_t n)
{
    idx_t count = 0;
    seq_t val = 1;
    while (count < n) {
        if (_a044912_is_valid(val)) {
            count++;
            if (count == n) return val;
        }
        val++;
    }
    return 0;   /* unreachable */
}
