/*
 * A039120  Numbers whose base-10 representation has the same number of 1's
 *          and 2's.
 * https://oeis.org/A039120
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) is the n‑th non‑negative integer whose decimal
 *            representation contains an equal number of the digits 1 and 2
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   digit_frequencies
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/digit_frequencies.h"

seq_t a(idx_t n)
{
    idx_t count = 0;
    idx_t i = 0;
    while (1) {
        seq_t val = (seq_t)i;
        unsigned int freq[10];
        digit_frequencies(val, freq);
        if (freq[1] == freq[2]) {
            ++count;
            if (count == n)
                return val;
        }
        ++i;
    }
}
