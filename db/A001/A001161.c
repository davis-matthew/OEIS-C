/*
 * A001161  Numbers containing an even number of letters.
 * https://oeis.org/A001161
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, word
 * Formula:   search -- a(n) is the n-th k >= 0 whose English name (no "and",
 *            American short scale) has an even number of letters
 * Range:     u8 n <= 50, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   english_letter_count
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/english_letter_count.h"

seq_t a(idx_t n) {
    idx_t count = 0;
    seq_t num = 0;

    while (1) {
        if (english_letter_count(num) % 2 == 0) {
            count++;
            if (count == n) return num;
        }
        num++;
    }
}
