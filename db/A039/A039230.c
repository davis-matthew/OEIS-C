/*
 * A039230  Numbers whose base-12 representation has the same number of 2's
 *          and 3's.
 * https://oeis.org/A039230
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Validated: partial, pass, 69 terms through n = 69 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Return true if ‘value’ has the same number of digits 2 and 3 in base 12 */
static bool equal_23(seq_t value)
{
    seq_t t = value;
    int count2 = 0;
    int count3 = 0;
    while (t != (seq_t)0) {
        seq_t digit = t % (seq_t)12;
        if (digit == (seq_t)2)
            count2++;
        else if (digit == (seq_t)3)
            count3++;
        t = t / (seq_t)12;
    }
    return count2 == count3;
}

/* Search: enumerates integers and keeps those whose base‑12 representation
   contains an equal number of 2s and 3s. */

seq_t a(idx_t n)
{
    idx_t found = 0;
    seq_t current = (seq_t)0;
    while (true) {
        if (equal_23(current)) {
            found++;
            if (found == n)
                return current;
        }
        current = current + (seq_t)1;
    }
}
