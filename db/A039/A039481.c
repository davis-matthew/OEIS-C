/*
 * A039481  Numbers whose base-11 representation has the same nonzero number
 *          of 5's and 9's.
 * https://oeis.org/A039481
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   equal_fives_nines
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/equal_fives_nines.h"
/* Sequence A039481: Numbers whose base‑11 representation has the same
   nonzero number of 5's and 9's.  */

seq_t a(idx_t n)
{
    seq_t val = 0;
    idx_t count = 0;
    while (count < n) {
        if (equal_fives_nines(val)) {
            ++count;
        }
        val = val + 1;
    }
    return val - 1;
}
