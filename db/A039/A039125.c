/*
 * A039125  Numbers whose base-10 representation has the same number of 1's
 *          and 7's.
 * https://oeis.org/A039125
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 120, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   digit_frequencies
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A039125: Numbers whose base-10 representation has the same
   number of 1's and 7's.  The function returns the n-th member of the
   sequence (1-indexed).  The algorithm scans through successive
   integers and selects those whose digit counts satisfy
   count('1') == count('7'). */
#include "../utils/digit_frequencies.h"

seq_t a(idx_t n) {
    if (n < 1) return 0;   /* Not defined for n < 1, but return 0 safely */
    idx_t found = 0;
    seq_t current = 0;
    unsigned int freq[10];

    for (;;) {
        digit_frequencies(current, freq);
        if (freq[1] == freq[7]) {
            ++found;
            if (found == n) return current;
        }
        ++current;
    }
}
