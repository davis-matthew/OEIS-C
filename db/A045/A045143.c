/*
 * A045143  Numbers whose base-4 representation contains exactly two 2's and
 *          no 3's.
 * https://oeis.org/A045143
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 24, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   has_valid_base4_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A045143: numbers whose base‑4 representation contains exactly
   two 2's and no 3's.  The function generates the sequence by scanning
   integers from 0 upwards and testing the base‑4 digits.  The test
   stops as soon as the n‑th term is found. */

#include <stdbool.h>
#include "../utils/has_valid_base4_digits.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* undefined for n<1, return 0 */
    idx_t  seen = 0;
    seq_t  val  = 0;

    for (;;) {
        if (has_valid_base4_digits(val)) {
            seen++;
            if (seen == n) return val;
        }
        val++;
    }
}
