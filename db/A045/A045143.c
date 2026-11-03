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
 * Validated: partial, pass, 48 terms through n = 48 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A045143: numbers whose base‑4 representation contains exactly
   two 2's and no 3's.  The function generates the sequence by scanning
   integers from 0 upwards and testing the base‑4 digits.  The test
   stops as soon as the n‑th term is found. */

#include <stdbool.h>

static inline int has_valid_base4_digits(seq_t x)
{
    int cnt2 = 0;
    while (x != 0) {
        seq_t d = x % 4;
        if (d == 2) {
            cnt2++;
            if (cnt2 > 2) return 0;
        } else if (d == 3) {
            return 0;
        }
        x /= 4;
    }
    return cnt2 == 2;
}

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
