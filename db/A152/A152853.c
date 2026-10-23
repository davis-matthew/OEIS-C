/*
 * A152853  Numbers of form 21k+1 or 21k+2 that use only digits 3 and 7.
 * https://oeis.org/A152853
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 3, u32 n <= 27, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: a(n) = the n-th integer of the form 21k+1 or 21k+2 that
   consists solely of the decimal digits 3 and 7.  The function
   enumerates numbers of increasing length, checks each for the
   required congruence, and stops as soon as the n‑th qualifying
   value is found. */
#include <stdlib.h>

seq_t a(idx_t n) {
    if (n < 1) return 0;                     // undefined for n=0

    idx_t count = 0;                         // how many valid numbers found

    /* Enumerate numbers by increasing number of digits. */
    for (idx_t len = 1; ; ++len) {
        /* Total numbers of this length: 2^len (digits 3 or 7). */
        idx_t total = (idx_t)1 << len;

        for (idx_t mask = 0; mask < total; ++mask) {
            seq_t val = 0;                    // current number

            /* Build the number digit by digit. */
            for (int j = (int)len - 1; j >= 0; --j) {
                seq_t digit = (mask >> j & 1) ? 7 : 3;
                val = val * 10 + digit;
            }

            /* Accept numbers that are 1 or 2 modulo 21. */
            if (val % 21 == 1 || val % 21 == 2) {
                ++count;
                if (count == n) return val;
            }
        }
    }
}
