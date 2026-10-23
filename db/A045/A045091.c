/*
 * A045091  Numbers whose base-4 representation contains exactly one 1 and one
 *          2.
 * https://oeis.org/A045091
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 48, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Numbers whose base‑4 representation contains exactly one 1 and one 2
   Implementation: brute‑force search.  The function scans positive integers
   from 1 upwards, counts the digits 1 and 2 in each base‑4 representation,
   and returns the n‑th integer for which those counts are exactly one each. */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Count of terms found satisfying the property. */
    idx_t found = 0;

    /* Current integer to test.  Cur is 64‑bit; cast to seq_t only for return. */
    uint64_t cur = 0;

    while (found < n) {
        ++cur;

        uint64_t x = cur;
        int ones  = 0;
        int twos  = 0;

        /* Examine base‑4 digits. */
        while (x > 0) {
            int d = x % 4;
            if (d == 1) ++ones;
            else if (d == 2) ++twos;
            x /= 4;
        }

        if (ones == 1 && twos == 1) {
            ++found;
        }
    }

    return (seq_t)cur;
}
